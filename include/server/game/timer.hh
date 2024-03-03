#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class Timer {
public:
  Timer(int limit_seconds);

  bool is_finished() const;

  // Method to start the timer, and once it finished it calls the function given
  // in parameter
  void start(std::function<void()> callback);

  void reset();

  void stop();

  int get_time() const;

private:
  const int _limit_seconds;
  int _current_time;
  bool _is_running;
};