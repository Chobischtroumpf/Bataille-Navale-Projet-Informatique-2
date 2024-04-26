#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class Timer {
private:
  int _limit_seconds;
  std::atomic<int> _current_time;
  std::atomic<bool> _is_running;
  std::function<void()> _callback;
  std::atomic<bool> launched;

public:
  Timer();

  Timer(int limit_seconds, std::function<void()> callback);

  bool isFinished() const;

  // Method to start the timer
  void start();

  void reset();

  void stop();

  int getTime() const;

  int getOriginalTime() const;

  void set(int new_limit_seconds, std::function<void()> callback_function);
};

