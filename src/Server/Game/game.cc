#include "game.hh"

Game::Game(const nlohmann::json &game_details)
    : _board{std::make_shared<Board>()}, update_player1{true}, update_player2{true} {
  set_game(game_details);
  initialize_ship_placements();
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
    update_player1 = true;
    update_player2 = true;
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
    change_turn();
    update_player1 = true;
    update_player2 = true;
    
    return true;
  } else {
    // handle if the it s not his turn
    return false;
  }
}

nlohmann::json Game::get_state(PlayerRole player) {

  json game_json;
  
  if (player == PlayerRole::Leader && update_player1){
    game_json = _board->toJson(player);
    update_player1 = false;

  }else if(player == PlayerRole::Opponent && update_player2){
    game_json = _board->toJson(player);
    update_player2 = false;
    
  }else if(player == PlayerRole::Spectator){
    game_json = _board->toJson(player);
  }
  else{
    game_json["fleetA"] = "None";
    game_json["fleetB"] = "None";
  }

  if (is_finished()) {
      game_json["Finished"] = "true";
      if (game_timer.winner() == 0){
        if (_board->isVictory()){
          game_json["Winner"] = "PLAYERONE";
        } else{
          game_json["Winner"] = "PLAYERTWO";
        }
      }
      if (game_timer.winner() == 1) {
        game_json["Winner"] = "PLAYERONE";
      } else {
        game_json["Winner"] = "PLAYERTWO";
      }
  } else {
      game_json["Finished"] = "false";
      game_json["Winner"] = "None";
  }


  if (ship_placements_finished()){
    game_json["ship_placements_finished"] = "true";
  }else{
    game_json["ship_placements_finished"] = "false";
  }

  game_json["player1_timer"] = std::to_string(game_timer.get_player1_timer());
  game_json["player2_timer"] = std::to_string(game_timer.get_player2_timer());
  game_json["timer"] = std::to_string(game_timer.get_timer());
  if (_board->whoseTurn() == PLAYERONE){
    game_json["turn"] = "PLAYERONE";
  }else{
    game_json["turn"] = "PLAYERTWO";
  }
  

  return game_json;
}

void Game::set_game(const nlohmann::json &game_details) {
  
  mode_commandant = !(game_details["gamemode"].get<std::string>() == "Classic");
  
  int switch_time =
      game_details.at("turnTimeLimit").get<int>();
  int player_time =
      game_details.at("playerTimeLimit").get<int>();
  game_timer.set(switch_time, player_time, [this]() { change_turn(); });
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

void Game::change_turn(){
  _board->changeTurn();
  game_timer.switch_turn();
}

