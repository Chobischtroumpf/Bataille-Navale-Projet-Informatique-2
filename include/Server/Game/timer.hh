#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class Timer {
public:

  Timer();
  Timer(int limit_seconds);

  bool is_finished() const;

  // Method to start the timer, and once it finished it calls the function given
  // in parameter
  void start(std::function<void()> callback);

  void reset();

  void stop();

  int get_time() const;

  void set(int limit_seconds);

private:
  const int limit_seconds;
  int current_time;
  bool is_running;
};