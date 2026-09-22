// recurring_scheduled_thread_pool.cpp
//
// Thread pool + a scheduler that fires a task every day at a predetermined
// wall-clock time (e.g. 16:15 local time), running it on the pool each time.
//
// Build:
//   g++ -std=c++20 -O2 -pthread recurring_scheduled_thread_pool.cpp -o recurring_scheduler
//
// Notes:
//   - Uses std::chrono::system_clock for wall-clock target computation, so
//     it tracks local time correctly across DST transitions (each day's
//     target is recomputed from hour/minute/second via localtime, not by
//     blindly adding 24h to a time_point).
//   - Missed-fire handling: if the process was asleep/stopped past the
//     target time, on start it will fire "late" once (immediately) if
//     `catch_up_if_missed` is true, otherwise it rolls to the next day.
//   - Safe to stop cleanly via `stop()`; destructor also stops.

#ifdef THREADPOOL_SESSION_SCHEDULER
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// ----------------------------- Thread Pool ------------------------------

class ThreadPoolSessionScheduler {
public:
    explicit ThreadPoolSessionScheduler(size_t num_threads = std::thread::hardware_concurrency())
    {
        if (num_threads == 0) num_threads = 1;
        workers_.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    ~ThreadPoolSessionScheduler()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& t : workers_) {
            if (t.joinable()) t.join();
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using Ret = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<Ret()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<Ret> fut = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stop_) throw std::runtime_error("enqueue on stopped ThreadPoolSessionScheduler");
            tasks_.emplace([task] { (*task)(); });
        }
        cv_.notify_one();
        return fut;
    }

private:
    void worker_loop()
    {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                if (stop_ && tasks_.empty()) return;
                job = std::move(tasks_.front());
                tasks_.pop();
            }
            job();
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
};

// ----------------------------- Time helpers -------------------------------

// Next occurrence (system_clock time_point) of local hour:minute:second.
// If `after` (default: now) is already past today's slot, rolls to tomorrow.
std::chrono::system_clock::time_point
next_time_at(int hour, int minute, int second = 0,
             std::chrono::system_clock::time_point after =
                 std::chrono::system_clock::now())
{
    using namespace std::chrono;

    std::time_t after_c = system_clock::to_time_t(after);
    std::tm local_tm{};
#if defined(_WIN32)
    localtime_s(&local_tm, &after_c);
#else
    localtime_r(&after_c, &local_tm);
#endif

    local_tm.tm_hour = hour;
    local_tm.tm_min  = minute;
    local_tm.tm_sec  = second;
    local_tm.tm_isdst = -1; // let mktime figure out DST for that date

    std::time_t target_c = std::mktime(&local_tm);
    auto target = system_clock::from_time_t(target_c);

    if (target <= after) {
        // Add a day by walking tm_mday forward and re-normalizing via
        // mktime, rather than adding a fixed 24h, so DST shifts are
        // handled correctly.
        local_tm.tm_mday += 1;
        local_tm.tm_isdst = -1;
        target_c = std::mktime(&local_tm);
        target = system_clock::from_time_t(target_c);
    }
    return target;
}

// ------------------------- Recurring scheduler -----------------------------

class DailyScheduledTask {
public:
    // hour/minute/second: local wall-clock fire time, every day.
    // catch_up_if_missed: if true and we start (or wake) past today's slot
    //   by more than a few seconds, fire immediately once, then resume the
    //   normal daily cadence from the following day.
    DailyScheduledTask(ThreadPoolSessionScheduler& pool,
                        int hour, int minute, int second,
                        std::function<void()> task,
                        bool catch_up_if_missed = false)
        : pool_(pool)
        , hour_(hour), minute_(minute), second_(second)
        , task_(std::move(task))
        , catch_up_if_missed_(catch_up_if_missed)
    {
        worker_ = std::thread([this] { run(); });
    }

    ~DailyScheduledTask() { stop(); if (worker_.joinable()) worker_.join(); }

    void stop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) return;
        stopped_ = true;
        cv_.notify_all();
    }

    // Next scheduled fire time (for logging/inspection).
    std::chrono::system_clock::time_point next_fire_time() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return next_;
    }

private:
    void run()
    {
        using namespace std::chrono;

        std::unique_lock<std::mutex> lock(mutex_);

        auto now = system_clock::now();
        next_ = next_time_at(hour_, minute_, second_, now);

        // If we're starting right after today's slot already passed and
        // catch-up is requested, fire almost immediately instead of
        // waiting ~24h.
        if (catch_up_if_missed_) {
            auto todays_slot = next_ - hours(24); // candidate for "today"
            if (now >= todays_slot && now - todays_slot < hours(24)) {
                // We're within the same day, past the slot -> fire now.
                next_ = now;
            }
        }

        while (!stopped_) {
            bool fired_or_stopped = cv_.wait_until(lock, next_, [this] {
                return stopped_ || std::chrono::system_clock::now() >= next_;
            });
            (void)fired_or_stopped;

            if (stopped_) return;

            // Fire: enqueue a copy of the task onto the pool.
            lock.unlock();
            try {
                pool_.enqueue(task_);
            } catch (const std::exception& e) {
                std::cerr << "DailyScheduledTask: enqueue failed: "
                          << e.what() << "\n";
            }
            lock.lock();

            // Schedule the next occurrence strictly after the one that
            // just fired, so a slow tick doesn't cause a double-fire.
            next_ = next_time_at(hour_, minute_, second_,
                                  next_ + std::chrono::seconds(1));
        }
    }

    ThreadPoolSessionScheduler& pool_;
    int hour_, minute_, second_;
    std::function<void()> task_;
    bool catch_up_if_missed_;

    std::thread worker_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    bool stopped_ = false;
    std::chrono::system_clock::time_point next_;
};

// ------------------------------- Demo -------------------------------------
#ifdef IS_TESTING_AND_DEMO
int main()
{
    ThreadPoolSessionScheduler pool(4);

    // Fire every day at 16:15:00 local time.
    DailyScheduledTask daily(pool, 16, 15, 0, [] {
        std::cout << "[worker] Daily 4:15 PM task running on pool.\n";
        // ... do the actual work here ...
    });

    {
        auto t = std::chrono::system_clock::to_time_t(daily.next_fire_time());
        std::cout << "Next fire time: " << std::ctime(&t);
    }

    // Keep the process alive. In a real service, replace this with your
    // normal event loop / signal handling (e.g. wait on a shutdown signal).
    std::this_thread::sleep_for(std::chrono::hours(24 * 365));

    daily.stop();
    return 0;
}
#endif
