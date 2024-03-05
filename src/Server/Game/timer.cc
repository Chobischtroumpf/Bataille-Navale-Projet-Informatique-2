#include "../../../include/server/game/timer.hh"


Timer::Timer(): limit_seconds{0}, current_time{0}, is_running{false} {}

Timer::Timer(int limit_seconds)
    : limit_seconds{limit_seconds}, current_time{limit_seconds},
      is_running{false} {}

bool Timer::is_finished() const { return current_time <= 0; }

void Timer::start(std::function<void()> callback) {
  if (!is_running) {
    is_running = true;
    std::thread([this, callback = std::move(callback)]() mutable {
      while (!is_finished() && is_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --current_time;
      }
      callback();
    }).detach();
  }
}

void Timer::reset() { current_time = limit_seconds; }

void Timer::stop() { is_running = false; }

void Timer::set(int limit_seconds) {
    limit_seconds = limit_seconds;
    current_time = limit_seconds;
}

int Timer::get_time() const { return current_time; }
