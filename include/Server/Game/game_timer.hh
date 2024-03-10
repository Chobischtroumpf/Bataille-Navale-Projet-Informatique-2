#pragma once

#include "timer.hh"
#include "turn.hh"

class GameTimer {
public:

  GameTimer();

  GameTimer(int switch_time, int game_time);

  void start_timer();

  void switch_turn();

  bool is_finished() const;

  int get_player1_timer();

  int get_player2_timer();

  int get_timer() const;

  void set(int switch_time, int player_time, std::function<void()> callback_function);

  int winner() const;

private:
  Timer timer;
  std::atomic<int> player1_timer;
  std::atomic<int> player2_timer;
  Turn turn;
  std::atomic<bool> finished;

  void update_time();
};
