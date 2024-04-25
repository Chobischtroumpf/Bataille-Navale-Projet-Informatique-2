#include "timer.hh"

Timer::Timer() : limit_seconds{0}, current_time{0}, is_running{false} {}

Timer::Timer(int limit_seconds, std::function<void()> callback)
    : limit_seconds{limit_seconds}, current_time{limit_seconds},
      is_running{false}, callback{std::move(callback)} {}

bool Timer::isFinished() const { return current_time <= 0; }

void Timer::start() {
  if (!is_running) {
    is_running = true;
    std::thread([this]() {
      auto start_time = std::chrono::steady_clock::now();
      while (!isFinished() && is_running) {
        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(
                                   end_time - start_time)
                                   .count();

        if (elapsed_seconds >= 1) {
          --current_time;
          start_time = end_time;
        }
      }

      if (isFinished() && callback) {
        callback();
      }
    }).detach();
  }
}

void Timer::reset() {
  stop();
  current_time = limit_seconds;
}

void Timer::stop() { is_running = false; }

int Timer::get_time() const { return current_time; }

int Timer::get_original_time() const { return limit_seconds; }

void Timer::set(int new_limit_seconds, std::function<void()> callback_function) {
  limit_seconds = new_limit_seconds;
  current_time = limit_seconds;
  callback = callback_function;
}