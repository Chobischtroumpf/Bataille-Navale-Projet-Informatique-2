#include "game_state.hh"
#include "faction_bombardement.hh"
#include "faction_sonar.hh"
#include "faction_mines.hh"
#include <iostream>

GameState::GameState(const nlohmann::json& gameDetails) : _game{std::make_shared<Game>(gameDetails)} {
  /*// Initialize currentState with some default value or based on gameDetails
  currentState = nlohmann::json::object();*/
}

GameState::~GameState() {
  // Cleanup resources if needed
}

bool GameState::makeMove(PlayerRole player, const nlohmann::json& move) {
  std::clog << "GameState::makeMove: ";

  std::string str_move = move.at("moveType").get<std::string>();
  std::clog << "move type: " << str_move << std::endl;
  bool result = false;

  if (str_move == "EndGame") {
    std::clog << "endGame move" << std::endl;
    _phase = Phase::FINISHED;
    result = true;
  }
  if (_phase == Phase::NOT_STARTED) {
    std::clog << "not started phase: ";
    if (str_move == "StartGame") {
      std::clog << "startGame move" << std::endl;
      _phase = Phase::PLACING_SHIPS;
      result = true;
    }
    else if (str_move == "chooseFaction") {
      std::clog << "chooseFaction move" << std::endl;
      //handle choose faction
      result = handleChooseFaction(player, move);
    }
  }
  else if (_phase == Phase::PLACING_SHIPS) {
    std::clog << "placing ships phase: ";
    if (str_move == "placeShips") {
      std::clog << "placeShips move" << std::endl;
      result = handlePlaceShip(player, move.at("ships"));
    }
    if (_game->shipPlacementsFinished()) {
      _phase = Phase::PLAYING;
      result = true;
    }
  }
  else if (_phase == Phase::PLAYING) {
    std::clog << "playing phase: ";
    if (str_move == "fire") {
      std::clog << "fire move" << std::endl;
      result = handleFire(player, move.at("fire"));
    }
    // else if (str_move == "placeShips") {
    //   std::clog << "placeShips move" << std::endl;
    //   result = handlePlaceShip(player, move.at("ships"));
    // }
  }

  std::clog << "result: " << result << std::endl;
  return result;
}

bool GameState::handleFire(PlayerRole player, const nlohmann::json& fire_move) {
  std::clog << "GameState::handleFire" << std::endl;
  SpecialAbilityType ability_type = fire_move.at("ability").get<SpecialAbilityType>();
  size_t x = fire_move.at("anchor").at("x").get<size_t>();
  size_t y = fire_move.at("anchor").at("y").get<size_t>();
  BoardCoordinates board_coordinates{x, y};
  return _game->handleFire(role_to_turn(player), ability_type, board_coordinates);

}

bool GameState::handlePlaceShip(PlayerRole player, const nlohmann::json& ships) {
  std::clog << "GameState::handlePlaceShip" << std::endl;
  for (const auto& obj_ship : ships) {

    std::clog << "ship: " << obj_ship << std::endl;
    size_t top_left_x = obj_ship.at("anchor").at("x");
    size_t top_left_y = obj_ship.at("anchor").at("y");

    std::clog << "create BoardCoordinates at: " << top_left_x << " " << top_left_y << std::endl;
    BoardCoordinates top_left{top_left_x, top_left_y};

    std::clog << "create vector of BoardCoordinates" << std::endl;
    std::vector<BoardCoordinates> coords{};
    const auto arr_coordinates = obj_ship.at("coordinates");
    std::clog << "arr_coordinates: " << arr_coordinates << std::endl;
    for (const auto& elem : arr_coordinates) {
      size_t i = elem.at("x");
      size_t j = elem.at("y");
      BoardCoordinates board_coordinates{i, j};
      coords.push_back(board_coordinates);
    }

    bool side;
    if (role_to_turn(player) == Turn::PLAYERONE) {
      side = true;
    } else {
      side = false;
    }

    std::clog << "create Ship" << std::endl;
    Ship ship{top_left, coords, side};

    std::clog << "set Ship type" << std::endl;
    ship.setType(obj_ship.at("type")); // a mon avis ca ca va pas marcher, on a pas de conversion int -> CellType

    bool result = _game->handlePlaceShip(role_to_turn(player), ship, _phase);
    if (!result) {
      //error in placing ship gotta be handeled
      return false;
    }
  }
  return true;
}

bool GameState::handleChooseFaction(PlayerRole player, const nlohmann::json& move) {
  int faction = move.at("faction");
  Faction f;

  switch (faction) {
    case 0:
      f = FactionBombardement();
      break;
    case 1:
      f = FactionSonar();
      break;
    case 2:
      f = FactionMines();
      break;
    default:
      return false;
  }
  return _game->setPlayerFaction(player, f);
}

Turn GameState::role_to_turn(PlayerRole player) {
  if (player == PlayerRole::Leader) {
    return Turn::PLAYERONE;
  } else if (player == PlayerRole::Opponent) {
    return Turn::PLAYERTWO;
  } else {
    // Spectator has no turn
    // Return a default value
    return Turn::PLAYERONE;
  }
}

nlohmann::json GameState::getGameState(PlayerRole player) const {
  // Return a placeholder or current game state
  // For simplicity, we return currentState directly
  return _game->getState(player);
}

Phase GameState::getPhase() const {
  return _phase;
}

bool GameState::hasStarted() const {
  return _phase != Phase::NOT_STARTED;
}