#pragma once

#include "timer.hh"
#include "turn.hh"

class GameTimer {
protected:
  Timer _game_timer;
  Timer _player1_timer;
  Timer _player2_timer;
  Turn _turn;
  std::atomic<bool> _finished;
  int _winner;

public:
  GameTimer();
  void startTimer();
  bool isFinished() const;
  int getPlayer1Timer() const;
  int getPlayer2Timer() const;
  int getGameTimer() const;
  int getWinner() const;
  void playerTimeRunout();
  virtual void turnReset()= 0;
  virtual void set(int game_time, int player_time,
                   std::function<void()> callback_function = {}) = 0;
  virtual void switchTurn() = 0;
  virtual ~GameTimer() = default;
};
