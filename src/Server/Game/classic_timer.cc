#include "classic_timer.hh"

ClassicTimer::ClassicTimer() : GameTimer() {}

void ClassicTimer::gameTimeRunout() {
  if (!_finished) {
    _finished = true;
    _winner = 0;
  }
}

void ClassicTimer::set(int game_time, int player_time,
                       std::function<void()> callback_function) {
  (void)callback_function;
  _game_timer.set(game_time, [this]() { gameTimeRunout(); });
  _player1_timer.set(player_time, [this]() { playerTimeRunout(); });
  _player2_timer.set(player_time, [this]() { playerTimeRunout(); });
}

void ClassicTimer::turnReset() {
  if (!_finished) {
    if (_turn == PLAYERONE) {
      _player1_timer.reset();
      _player2_timer.start();
    } else {
      _player2_timer.reset();
      _player1_timer.start();
    }
    
  }
}

void ClassicTimer::switchTurn() {
  turnReset();
  _turn = (_turn == PLAYERONE) ? PLAYERTWO : PLAYERONE;
}