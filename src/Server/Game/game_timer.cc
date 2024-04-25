#include "game_timer.hh"

GameTimer::GameTimer():_game_timer{}, _player1_timer{},
      _player2_timer{}, _turn{PLAYERONE}, _finished{false}, _winner{0} {}

void GameTimer::startTimer() {
    _game_timer.start();
    _player1_timer.start();
}

bool GameTimer::isFinished() const { return _finished; }

int GameTimer::getPlayer1Timer() const { return _player1_timer.getTime(); }

int GameTimer::getPlayer2Timer() const { return _player2_timer.getTime(); }

int GameTimer::getGameTimer() const { return _game_timer.getTime(); }

int GameTimer::getWinner() const { return _winner; }

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