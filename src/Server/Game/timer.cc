#include "timer.hh"

Timer::Timer() : _limit_seconds{0}, _current_time{0}, _is_running{false}, launched{false} {}

Timer::Timer(int limit_seconds, std::function<void()> callback)
    : _limit_seconds{limit_seconds}, _current_time{limit_seconds},
      _is_running{false}, _callback{std::move(callback)}, launched{false} {}

bool Timer::isFinished() const { return _current_time <= 0; }

void Timer::start() {
  if (!launched) {
    launched = true;
    _is_running = true;
    std::thread([this]() {
      auto start_time = std::chrono::steady_clock::now();
      while (!isFinished()) {
        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(
                                   end_time - start_time)
                                   .count();

        if (elapsed_seconds >= 1 && _is_running) {
          --_current_time;
          start_time = end_time;
        }
      }
      launched = false;
      if (isFinished() && _callback) {
        _callback();
      }
    }).detach();
  }else{
    _is_running = true;
  }
}

void Timer::reset() {
  stop();
  _current_time = _limit_seconds;
}

void Timer::stop() { _is_running = false; }

int Timer::getTime() const { return _current_time; }

int Timer::getOriginalTime() const { return _limit_seconds; }

void Timer::set(int new_limit_seconds, std::function<void()> callback_function) {
  _limit_seconds = new_limit_seconds;
  _current_time = new_limit_seconds;
  _callback = callback_function;
}
