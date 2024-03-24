#include "game.hh"

Game::Game(const nlohmann::json &game_details)
    : _board{std::make_shared<Board>()}, _update_player1{true}, _update_player2{true} {
  setGame(game_details);
  initializeShipPlacements();
}

bool Game::isFinished() const {
  return _board->isFinished() || _game_timer.isFinished();
}

void Game::startTimer() {
  _game_timer.startTimer();
  // _game_timer.start(std::bind(&Game::_game_timer_finished, this));
  // player_timer.start(std::bind(&Game::player_timer_finished, this));
}

bool Game::handlePlaceShip(Turn turn, Ship ship) {
  if (!shipPlacementsFinished()) {
    if (turn == PLAYERONE) {
      _board->placeShip(ship, true);
      _ship_placements.at(PLAYERONE)++;
    } else {
      _board->placeShip(ship, false);
      _ship_placements.at(PLAYERTWO)++;
      
    }
    _update_player1 = true;
    _update_player2 = true;
    // if the ship placements finished then start the timer
    if (shipPlacementsFinished()){
      startTimer();
    }
    return true;
  }
  return false;
}

bool Game::handleFire(Turn turn, BoardCoordinates board_coordinates) {
  if (_board->whoseTurn() == turn) {
    // _board->fire(board_coordinates); // TODO: adapter au nouveau fire
    changeTurn();
    _update_player1 = true;
    _update_player2 = true;
    
    return true;
  } else {
    // handle if the it s not his turn
    return false;
  }
}

nlohmann::json Game::getState(PlayerRole player) {

  nlohmann::json game_json;
  
  if (player == PlayerRole::Leader && _update_player1){
    game_json = _board->to_json(player);
    _update_player1 = false;

  }else if(player == PlayerRole::Opponent && _update_player2){
    game_json = _board->to_json(player);
    _update_player2 = false;
    
  }else if(player == PlayerRole::Spectator){
    game_json = _board->to_json(player);
  }
  else{
    game_json["fleetA"] = "None";
    game_json["fleetB"] = "None";
  }

  if (isFinished()) {
      game_json["Finished"] = "true";
      if (!_game_timer.isFinished()){
        if (_board->isVictory()){
          game_json["Winner"] = "PLAYERONE";
        } else{
          game_json["Winner"] = "PLAYERTWO";
        }
      }else{
        if (_game_timer.getWinner() == 0){
          game_json["Winner"] = "INDECISIVE";
        }
        if (_game_timer.getWinner() == 1) {
          game_json["Winner"] = "PLAYERONE";
        } else {
          game_json["Winner"] = "PLAYERTWO";
        }
      }
      
  } else {
      game_json["Finished"] = "false";
      game_json["Winner"] = "None";
  }


  if (shipPlacementsFinished()){
    game_json["_ship_placements_finished"] = "true";
  }else{
    game_json["_ship_placements_finished"] = "false";
  }

  game_json["player1_timer"] = std::to_string(_game_timer.getPlayer1Timer());
  game_json["player2_timer"] = std::to_string(_game_timer.getPlayer2Timer());
  game_json["_game_timer"] = std::to_string(_game_timer.getGameTimer());
  if (_board->whoseTurn() == PLAYERONE){
    game_json["turn"] = "PLAYERONE";
  }else{
    game_json["turn"] = "PLAYERTWO";
  }

  game_json["gamemode"] = _mode_commandant ? "Commandant" : "Classic";
  

  return game_json;
}

void Game::setGame(const nlohmann::json &game_details) {
  
  _mode_commandant = !(game_details["gamemode"].get<std::string>() == "Classic");
  
  int game_time =
      game_details.at("turnTimeLimit").get<int>();
  int player_time =
      game_details.at("playerTimeLimit").get<int>();
  _game_timer.set(game_time, player_time);
  // player_timer.set(std::stoi(game_details.at("player_timer").get<std::string>()));
  // _game_timer.set(std::stoi(game_details.at("_game_timer").get<std::string>()));
}

void Game::initializeShipPlacements() {
  // Clear existing contents if any
  _ship_placements.clear();
  _ship_placements.emplace(PLAYERONE, 0);
  _ship_placements.emplace(PLAYERTWO, 0);
}

bool Game::shipPlacementsFinished() const {

  return _ship_placements.at(PLAYERONE) == _required__ship_placements &&
         _ship_placements.at(PLAYERTWO) == _required__ship_placements;
}

void Game::changeTurn(){
  _board->changeTurn();
  _game_timer.switchTurn();
}