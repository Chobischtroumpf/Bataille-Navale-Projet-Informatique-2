#include "game.hh"

Game::Game(const nlohmann::json& game_details)
    : _board{std::make_shared<Board>()}, is_timer_finished{
                                                            false} {
  set_game(game_details);
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

void Game::set_game(const nlohmann::json& game_details){
  try {
    mode_commandant = !(game_details.at("type").get<std::string>() == "Classic");
    player_timer.set(game_details.at("turnTimeLimit").get<int>() * 60);
    game_timer.set(game_details.at("gameTimeLimit").get<int>() * 60);
  } catch (const std::exception& e) {
    std::cerr << "Exception in set_game: " << e.what() << std::endl;
    // Optionally, handle the error or set default values
  }
}