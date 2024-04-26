#include "game.hh"
#include "faction_classique.hh"
#include "player.hh"
#include "special_ability.hh"
#include <vector>


void Game::startTimer() {
  _game_timer->startTimer();
  // _game_timer.start(std::bind(&Game::_game_timer_finished, this));
  // player_timer.start(std::bind(&Game::player_timer_finished, this));
}

void Game::setGame(const nlohmann::json &game_details) {
  
  _mode_commandant = !(game_details["gamemode"].get<std::string>() == "Classic");

  int game_time =
      game_details.at("turnTimeLimit").get<int>();
  int player_time =
      game_details.at("playerTimeLimit").get<int>();
  //bool classic_timer = true;
  bool classic_timer = game_details.at("classicTimer").get<std::string>() == "Classic";
  if (classic_timer){
    _game_timer = std::make_unique<ClassicTimer>();
    _game_timer->set(game_time, player_time);
  }else{
    _game_timer = std::make_unique<PendulumTimer>();
    _game_timer->set(player_time, game_time, [this]() { changeTurn(); });
  }
  //_game_timer->set(game_time, player_time);

}

void Game::changeTurn(){
  _board->changeTurn();
  _game_timer->switchTurn();
}

Game::Game(const nlohmann::json &game_details)
    : _board{std::make_shared<Board>()}, _game_started{false},
    _mode_commandant{false} {
  setGame(game_details);
}
 
bool Game::isFinished() const {
  return _board->isFinished() || _game_timer->isFinished();
}

bool Game::shipPlacementsFinished() const {

  Player player1 = _board->getPlayer1();
  Player player2 = _board->getPlayer2();

  // std::clog << "Player 1 fleet size : " << player1.getFleet().size() << std::endl;
  // std::clog << "Player 2 fleet size: " << player2.getFleet().size() << std::endl;
  // std::clog << "Player 1 faction amount of ships: " << player1.getFaction().getAmountOfShips() << std::endl;
  // std::clog << "Player 2 faction amount of ships: " << player2.getFaction().getAmountOfShips() << std::endl;

  if (player1.getFleet().size() >= (size_t)player1.getFaction().getAmountOfShips() &&
      player2.getFleet().size() >= (size_t)player2.getFaction().getAmountOfShips()) {
    return true;
  }
  return false;
}

bool Game::handlePlaceShip(Turn turn, Ship &ship, Phase phase) {
  std::clog << "GAME::HANDLEPLACESHIP" << std::endl;
  std::clog << "ship.top_left : " << ship.getTopLeft() << std::endl;
  if (phase == Phase::PLACING_SHIPS && !shipPlacementsFinished()) {
    ship.setBoard(_board.get());
    if (turn == PLAYERONE) {
      _board->placeShip(std::make_shared<Ship>(ship), true);
    } else {
      _board->placeShip(std::make_shared<Ship>(ship), false);
    }
    // if the ship placements finished then start the timer
    if (phase == Phase::PLACING_SHIPS && shipPlacementsFinished()){
        startTimer();
    }
    return true;
  }
  return false;
}

bool Game::handleFire(Turn turn, SpecialAbilityType ability_type, BoardCoordinates board_coordinates) {
  Player &player = (turn == PLAYERONE) ? _board->getPlayer1() : _board->getPlayer2();

  std::clog << "Firing ability: " << ability_type << std::endl;
  for (auto &ability : player.getFaction().getSpecialAbilities()) {
    std::clog << "Checking ability: " << ability.getType() << std::endl;
    std::clog << "Is it the same : " << (ability.getType() == ability_type) << std::endl;
    if (ability.getType() == ability_type) {
      std::clog << "Firing ability: " << ability.getName() << std::endl;
      if (_board->fire(ability, board_coordinates)) {
        _game_timer->turnReset();
        if (_mode_commandant){
          player.addEnergyPoints(1);
        }
      } else {
        changeTurn();
      }
      return true;
    }
  }
  return false;
}

bool Game::setPlayerFaction(PlayerRole player, Faction faction) {
  if (player == PlayerRole::Leader) {
    _board->getPlayer1().setFaction(faction);
  } else if (player == PlayerRole::Opponent) {
    _board->getPlayer2().setFaction(faction);
  } else {
    return false;
  }
  return true;
}

nlohmann::json Game::getState(PlayerRole player) {
  nlohmann::json game_json;
  
  game_json = _board->to_json(player);

  if (isFinished()) {
      game_json["Finished"] = "true";
      if (!_game_timer->isFinished()){
        if (_board->isVictory()){
          game_json["Winner"] = "PLAYERONE";
        } else{
          game_json["Winner"] = "PLAYERTWO";
        }
      }else{
        if (_game_timer->getWinner() == 0){
          game_json["Winner"] = "DRAW";
        }
        if (_game_timer->getWinner() == 1) {
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
    game_json["ship_placements_finished"] = "true";
  }else{
    game_json["ship_placements_finished"] = "false";
  }

  game_json["player1_timer"] = _game_timer->getPlayer1Timer();
  game_json["player2_timer"] = _game_timer->getPlayer2Timer();
  game_json["game_timer"] = _game_timer->getGameTimer();
  
  if (_board->whoseTurn() == PLAYERONE){
    game_json["turn"] = "PLAYERONE";
  }else{
    game_json["turn"] = "PLAYERTWO";
  }

  game_json["gamemode"] = _mode_commandant ? "Commandant" : "Classic";
  
  return game_json;
}
