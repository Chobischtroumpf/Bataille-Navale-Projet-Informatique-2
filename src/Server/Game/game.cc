#include "game.hh"

Game::Game(const nlohmann::json &game_details)
    : _board{std::make_shared<Board>()}, _update_player1{true}, _update_player2{true} {
  setGame(game_details);
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
      _update_player1 = true;
    } else {
      _board->placeShip(ship, false);
      _update_player2 = true;
    }

    // if the ship placements finished then start the timer
    if (shipPlacementsFinished()){
        startTimer();
    }
    return true;
  }
  return false;
}

bool Game::handleFire(Turn turn, SpecialAbilityType ability_type, BoardCoordinates board_coordinates) {
  if (_board->whoseTurn() == turn) {
    // handle the fire
    // _board->fire(
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
  
  // set the factions
  // string faction1 = game_details.at("faction1").get<std::string>();
  // string faction2 = game_details.at("faction2").get<std::string>();

  // if (faction1 == "Mines"){
  //   _board->getPlayer1().setFaction(FactionMines());
  // }else if (faction1 == "Sonar"){
  //   _board->getPlayer1().setFaction(FactionSonar());
  // }else if (faction1 == "Bombardement"){
  //   _board->getPlayer1().setFaction(FactionBombardement());
  // }else{
  //   _board->getPlayer1().setFaction(FactionClassique());
  // }

  // if (faction2 == "Mines"){
  //   _board->getPlayer2().setFaction(FactionMines());
  // }else if (faction2 == "Sonar"){
  //   _board->getPlayer2().setFaction(FactionSonar());
  // }else if (faction2 == "Bombardement"){
  //   _board->getPlayer2().setFaction(FactionBombardement());
  // }else{ 
  //   _board->getPlayer2().setFaction(FactionClassique());
  // }

  int game_time =
      game_details.at("turnTimeLimit").get<int>();
  int player_time =
      game_details.at("playerTimeLimit").get<int>();
  _game_timer.set(game_time, player_time);
  // player_timer.set(std::stoi(game_details.at("player_timer").get<std::string>()));
  // _game_timer.set(std::stoi(game_details.at("_game_timer").get<std::string>()));
}

bool Game::shipPlacementsFinished() const {

  Player player1 = _board->getPlayer1();
  Player player2 = _board->getPlayer2();

  std::cerr << "player1.getFleet().size() : " << player1.getFleet().size() << std::endl;
  std::cerr << "player1.getFaction().getAmountOfShips() : " << player1.getFaction().getAmountOfShips() << std::endl;
  std::cerr << "player2.getFleet().size() : " << player2.getFleet().size() << std::endl;
  std::cerr << "player2.getFaction().getAmountOfShips() : " << player2.getFaction().getAmountOfShips() << std::endl;
  std::cerr << "player1.getFleet().size() == player1.getFaction().getAmountOfShips() : " << (player1.getFleet().size() == player1.getFaction().getAmountOfShips()) << std::endl;
  std::cerr << "player2.getFleet().size() == player2.getFaction().getAmountOfShips() : " << (player2.getFleet().size() == player2.getFaction().getAmountOfShips()) << std::endl;

  if (player1.getFleet().size() == player1.getFaction().getAmountOfShips() &&
      player2.getFleet().size() == player2.getFaction().getAmountOfShips()) {
    return true;
  }
  return false;
}

void Game::changeTurn(){
  _board->changeTurn();
  _game_timer.switchTurn();
}