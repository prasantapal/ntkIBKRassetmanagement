struct EventHandler : StateController {
  EventHandler();
  ~EventHandler();

  void start_listening_to_events();
  std::unique_ptr<std::jthread> event_handler_thread_;

  bool should_still_handle_events;
  void  ctor_helper();
};

void  EventHandler::ctor_helper() {
  should_still_handle_events = true;
}

EventHandler::EventHandler() {
  ctor_helper();

}
EventHandler::~EventHandler() {

}


void EventHandler::start_listening_to_events() {


  auto event_handler_function = [this]()->void {



    // std::lock_guard<std::mutex> lock(mtx);
    //    std::unique_lock<std::mutex> lock(mtx);
    //    std::cout << "ACQUIRED LOCK:" << data_ready  << std::endl;
    //    cv.wait(lock, [this]{ return data_ready; });
    //    {
    //      std::cout << "EVENT IS ACTIVE " << std::endl;
    //
    //    }
    //

    std::cout << "started listening to events" << std::endl;
    while(should_still_handle_events) {
      std::unique_lock<std::mutex> lock(mtx);


      // std::lock_guard<std::mutex> lock(mtx);
      cv.wait(lock, [this]{ return data_ready; });
      {
        std::cout << "EVENT TRIGGER WITH VALUE :\n" << json_FIFO << std::endl;
        // 2. Wait until data_ready is true.
        // Pass the lock and a lambda function (predicate).
        //cv.wait(lock, [this] { 
        //    std::cout << "EVENT IS ACTIVE " << std::endl;
        //    getchar();
        //    return data_ready;
        //    });

        // 3. Once unblocked, we own the lock and can safely consume the data

        data_ready = false;
      }

    }

  };

  event_handler_thread_.reset(new std::jthread(event_handler_function));
}
