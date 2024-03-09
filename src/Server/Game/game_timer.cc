#include "game_timer.hh"

GameTimer::GameTimer():timer{}, player1_timer{0}, player2_timer{0},turn{PLAYERONE}, finished{false}{}

GameTimer::GameTimer(int switch_time, int game_time)
    : timer{switch_time, [this]() { switch_turn(); }}, player1_timer{game_time},
      player2_timer{game_time}, turn{PLAYERONE}, finished{false} {}

void GameTimer::start_timer() { timer.start(); }

void GameTimer::switch_turn() {
  timer.stop();
  update_time();
  turn = (turn == PLAYERONE) ? PLAYERTWO : PLAYERONE;
  timer.reset();
  timer.start();
}

bool GameTimer::is_finished() const { return finished; }

int GameTimer::get_player1_timer() {
  int temp_timer = player1_timer - timer.get_original_time() + timer.get_time();
  if (temp_timer <= 0) {
    finished = true;
    temp_timer = 0;
  }
  return temp_timer;
}

int GameTimer::get_player2_timer() {
  int temp_timer = player2_timer - timer.get_original_time() + timer.get_time();
  if (temp_timer <= 0) {
    finished = true;
    temp_timer = 0;
  }
  return temp_timer;
}

int GameTimer::get_timer() const { return timer.get_time(); }

void GameTimer::update_time() {
  if (turn == PLAYERONE) {
    player1_timer =
        player1_timer - timer.get_original_time() + timer.get_time();
    if (player1_timer <= 0) {
      finished = true;
      player1_timer = 0;
    }
  } else {
    player2_timer =
        player2_timer - timer.get_original_time() + timer.get_time();
    if (player2_timer <= 0) {
      finished = true;
      player2_timer = 0;
    }
  }
}

void GameTimer::set(int switch_time, int player_time){
    timer.set(switch_time, [this]() { switch_turn(); });
    player1_timer = player_time;
    player2_timer = player_time;
}
