#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>

class Timer {
public:

  Timer();

  Timer(int limit_seconds, std::function<void()> callback);

  bool is_finished() const;

  // Method to start the timer
  void start();

  void reset();

  void stop();

  int get_time() const;

  int get_original_time() const;

  void set(int new_limit_seconds, std::function<void()> callback_function);

private:
  int limit_seconds;
  std::atomic<int> current_time;
  std::atomic<bool> is_running;
  std::function<void()> callback;
};