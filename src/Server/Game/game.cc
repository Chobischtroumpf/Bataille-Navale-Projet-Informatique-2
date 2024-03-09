#include "game.hh"

Game::Game(const nlohmann::json &game_details)
    : _board{std::make_shared<Board>()} {
  set_game(game_details);
  initialize_ship_placements();
  start_timer();
}

bool Game::is_finished() const {
  return _board->isFinished() || game_timer.is_finished();
}

void Game::start_timer() {
  game_timer.start_timer();
  // game_timer.start(std::bind(&Game::game_timer_finished, this));
  // player_timer.start(std::bind(&Game::player_timer_finished, this));
}

bool Game::handle_place_ship(Turn turn, ShipCoordinates ship_coordinates) {
  if (!ship_placements_finished()) {
    if (turn == PLAYERONE) {
      _board->placeShip(ship_coordinates, true);
      ship_placements.at(PLAYERONE)++;
    } else {
      _board->placeShip(ship_coordinates, false);
      ship_placements.at(PLAYERTWO)++;
    }
    // if the ship placements finished then start the timer
    if (ship_placements_finished()){
      start_timer();
    }
    return true;
  }
  return false;
}

bool Game::handle_fire(Turn turn, BoardCoordinates board_coordinates) {
  if (_board->whoseTurn() == turn) {
    _board->fire(board_coordinates);
    _board->changeTurn();
    game_timer.switch_turn();
    return true;
  } else {
    // handle if the it s not his turn
    return false;
  }
}

nlohmann::json Game::get_state(PlayerRole player) {
  return _board->toJson(player);
}

void Game::set_game(const nlohmann::json &game_details) {
  std::string str_bool_commandant =
      game_details.at("mode_commandant").get<std::string>();
  if (str_bool_commandant == "true") {
    mode_commandant = true;
  } else if (str_bool_commandant == "false") {
    mode_commandant = false;
  } else {
    // handle error properly
    mode_commandant = false;
  }
  int switch_time =
      std::stoi(game_details.at("switch_time").get<std::string>());
  int player_time =
      std::stoi(game_details.at("player_time").get<std::string>());
  game_timer.set(switch_time, player_time);
  // player_timer.set(std::stoi(game_details.at("player_timer").get<std::string>()));
  // game_timer.set(std::stoi(game_details.at("game_timer").get<std::string>()));
}

void Game::initialize_ship_placements() {
  // Clear existing contents if any
  ship_placements.clear();
  ship_placements.emplace(PLAYERONE, 0);
  ship_placements.emplace(PLAYERTWO, 0);
}

bool Game::ship_placements_finished() const {

  return ship_placements.at(PLAYERONE) == required_ship_placements &&
         ship_placements.at(PLAYERTWO) == required_ship_placements;
}
