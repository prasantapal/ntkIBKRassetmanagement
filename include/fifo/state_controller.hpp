#ifndef STATE_CONTROLLER_HPP
#define STATE_CONTROLLER_HPP
#include <json/json.h>
#include <condition_variable>
#include <thread>
#include <mutex>

struct StateController {
  static std::mutex json_ready_mtx_;
  static std::condition_variable json_ready_CV_;
  static Json::Value json_FIFO;
  static bool is_json_ready_ ;
  StateController();
  ~StateController();
};

bool StateController::is_json_ready_ = {false};
std::condition_variable StateController::json_ready_CV_;
std::mutex StateController::json_ready_mtx_;
Json::Value StateController::json_FIFO;

StateController::StateController() {

  // is_json_ready_ = false;

}
StateController::~StateController() {

}

#endif
