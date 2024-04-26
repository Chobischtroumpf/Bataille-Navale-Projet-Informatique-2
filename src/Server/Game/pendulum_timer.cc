#include "pendulum_timer.hh"

PendulumTimer::PendulumTimer() : GameTimer() {}

void PendulumTimer::set(int game_time, int player_time,
                        std::function<void()> callback_function) {
  _game_timer.set(game_time, callback_function);
  _player1_timer.set(player_time, [this]() { playerTimeRunout(); });
  _player2_timer.set(player_time, [this]() { playerTimeRunout(); });
}

void PendulumTimer::turnReset() {
  if (!_finished) {
    if (_turn == PLAYERONE) {
      _player1_timer.stop();
      _game_timer.reset();
      _game_timer.start();
      _player1_timer.start();
    } else {
      _player2_timer.stop();
      _game_timer.reset();
      _game_timer.start();
      _player2_timer.start();
    }
  }
}

void PendulumTimer::switchTurn() {
    if (!_finished) {
    if (_turn == PLAYERONE) {
      _player1_timer.stop();
      _game_timer.reset();
      _game_timer.start();
      _player2_timer.start();
    } else {
      _player2_timer.stop();
      _game_timer.reset();
      _game_timer.start();
      _player1_timer.start();
    }
  }
  _turn = (_turn == PLAYERONE) ? PLAYERTWO : PLAYERONE;
  
}