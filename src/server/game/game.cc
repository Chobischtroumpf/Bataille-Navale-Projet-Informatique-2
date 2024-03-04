#include "../../../include/server/game/game.hh"

Game::Game(bool mode_commandant, int time_game, int time_player)
    : _board{std::make_shared<Board>()}, mode_commandant{mode_commandant},
      game_timer{time_game}, player_timer{time_player}, is_timer_finished{
                                                            false} {
  start_timer();
}

bool Game::is_finished() const {
  return _board->isFinished() || is_timer_finished;
}

void Game::start_timer() {
  game_timer.start(std::bind(&Game::game_timer_finished, this));
  player_timer.start(std::bind(&Game::player_timer_finished, this));
}

void Game::game_timer_finished() {
  is_timer_finished = true;
  // Handle it by sending both players to disconnect
}

void Game::player_timer_finished() {
  is_timer_finished = true;
  // Handle it by checking who s turn and setting the other as winner, might
  // use disconnect player
}

bool Game::handle_place_ship(Turn turn, ShipCoordinates ship_coordinates) {

  if (_board->whoseTurn() == turn) {
    _board->placeShip(ship_coordinates, _board->myTurn());
    _board->changeTurn();
    return true;
  } else {
    // handle if the it s not his turn
    return false;
  }
  // (Inform the player that the boat was placed sucessfully or only send the
  // json back ?)
}

bool Game::handle_fire(Turn turn, BoardCoordinates board_coordinates) {
  if (_board->whoseTurn() == turn) {
    _board->fire(board_coordinates);
    _board->changeTurn();
    return true;
  } else {
    // handle if the it s not his turn
    return false;
  }
}

nlohmann::json Game::get_state(PlayerRole player) {
  return _board->toJson(player);
}