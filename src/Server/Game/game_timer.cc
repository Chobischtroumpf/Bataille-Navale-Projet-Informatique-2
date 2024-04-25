#include "game_timer.hh"

GameTimer::GameTimer()
    : _game_timer{}, _player1_timer{},
      _player2_timer{}, _turn{PLAYERONE}, _finished{false}, _winner{0} {}

void GameTimer::startTimer() {
  _game_timer.start();
  _player1_timer.start();
}

void GameTimer::switchTurn() {
  if (!_finished) {
    if (_turn == PLAYERONE) {
      _player1_timer.stop();
      _player1_timer.reset();
      _player2_timer.start();
    } else {
      _player2_timer.stop();
      _player2_timer.reset();
      _player1_timer.start();
    }
    _turn = (_turn == PLAYERONE) ? PLAYERTWO : PLAYERONE;
  }
}

bool GameTimer::isFinished() const { return _finished; }

int GameTimer::getPlayer1Timer() const { return _player1_timer.get_time(); }

int GameTimer::getPlayer2Timer() const { return _player2_timer.get_time(); }

int GameTimer::getGameTimer() const { return _game_timer.get_time(); }

void GameTimer::playerTimeRunout() {
  if (!_finished) {
    _finished = true;
    if (_turn == PLAYERONE) {
      _winner = 2;
    } else {
      _winner = 1;
    }
  }
}

void GameTimer::set(int game_time, int player_time) {
  _game_timer.set(game_time, [this]() { gameTimeRunout(); });
  _player1_timer.set(player_time, [this]() { playerTimeRunout(); });
  _player2_timer.set(player_time, [this]() { playerTimeRunout(); });
}

int GameTimer::getWinner() const { return _winner; }

void GameTimer::gameTimeRunout() {
  if (!_finished) {
    _finished = true;
    _winner = 0;
  }
}