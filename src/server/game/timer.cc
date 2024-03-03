#include "timer.hh"

Timer::Timer(int limit_seconds)
    : _limit_seconds(limit_seconds), _current_time(limit_seconds),
      _is_running(false) {}

bool Timer::is_finished() const { return _current_time <= 0; }

void Timer::start(std::function<void()> callback) {
  if (!_is_running) {
    _is_running = true;
    std::thread([this, callback = std::move(callback)]() mutable {
      while (!is_finished() && _is_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --_current_time;
      }
      callback();
    }).detach();
  }
}

void Timer::reset() { _current_time = _limit_seconds; }

void Timer::stop() { _is_running = false; }

int Timer::get_time() const { return _current_time; }
