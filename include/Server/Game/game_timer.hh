#pragma once

#include "timer.hh"
#include "turn.hh"

class GameTimer {
  private:
    Timer _game_timer;
    Timer _player1_timer;
    Timer _player2_timer;
    Turn _turn;
    std::atomic<bool> _finished;
    int _winner;

    void playerTimeRunout();

    void gameTimeRunout();
  public:

    GameTimer();

    void startTimer();

    void switchTurn();

    bool isFinished() const;

    int getPlayer1Timer() const;

    int getPlayer2Timer() const;

    int getGameTimer() const;

    void set(int game_time, int player_time);

    int getWinner() const;

};

