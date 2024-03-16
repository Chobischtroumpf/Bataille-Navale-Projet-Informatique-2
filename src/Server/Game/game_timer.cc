#include "game_timer.hh"

GameTimer::GameTimer()
    : game_timer{}, player1_timer{},
      player2_timer{}, turn{PLAYERONE}, finished{false}, winner{0} {}

void GameTimer::start_timer() {
  game_timer.start();
  player1_timer.start();
}

void GameTimer::switch_turn() {
  if (!finished) {
    if (turn == PLAYERONE) {
      player1_timer.stop();
      player1_timer.reset();
      player2_timer.start();
    } else {
      player2_timer.stop();
      player2_timer.reset();
      player1_timer.start();
    }
    turn = (turn == PLAYERONE) ? PLAYERTWO : PLAYERONE;
  }
}

bool GameTimer::is_finished() const { return finished; }

int GameTimer::get_player1_timer() const { return player1_timer.get_time(); }

int GameTimer::get_player2_timer() const { return player2_timer.get_time(); }

int GameTimer::get_game_timer() const { return game_timer.get_time(); }

void GameTimer::player_time_runout() {
  if (!finished) {
    finished = true;
    if (turn == PLAYERONE) {
      winner = 2;
    } else {
      winner = 1;
    }
  }
}

void GameTimer::set(int game_time, int player_time) {
  game_timer.set(game_time, [this]() { game_time_runout(); });
  player1_timer.set(player_time, [this]() { player_time_runout(); });
  player2_timer.set(player_time, [this]() { player_time_runout(); });
}

int GameTimer::get_winner() const { return winner; }

void GameTimer::game_time_runout() {
  if (!finished) {
    finished = true;
    winner = 0;
  }
}