#pragma once

#include "timer.hh"
#include "turn.hh"

class GameTimer {
public:

  GameTimer();

  void start_timer();

  void switch_turn();

  bool is_finished() const;

  int get_player1_timer() const;

  int get_player2_timer() const;

  int get_game_timer() const;

  void set(int game_time, int player_time);

  int get_winner() const;

private:
  Timer game_timer;
  Timer player1_timer;
  Timer player2_timer;
  Turn turn;
  std::atomic<bool> finished;
  int winner;

  void player_time_runout();

  void game_time_runout();
};

