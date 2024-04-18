#include "local_board_commander.hh"
// #include <unistd.h>
// #include <iostream>

/*

AJOUTER SESSIONID DANS LE CONSTRUCTEUR

*/
LocalBoardCommander::LocalBoardCommander(std::shared_ptr<GameClient> client,
                                         Player player, GameMode mode,
                                         const std::string &sessionId)
    : _player{player}, _mode{mode}, _is_finished{false},
      _is_victory{false}, _my_board{std::vector<std::vector<Cell>>(
                              10, std::vector<Cell>(10, Cell()))},
      _their_board{
          std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))},
      _session_id{sessionId},
      _client{client} {
  auto futureMessages = _client->QueryGameState(sessionId);
  auto messagesJson = futureMessages.get();
  auto usersID = messagesJson["participants"];
  _my_username = _client->getClientUsername();
  std::string my_id = _client->GetUserId(_my_username).get();
  _player.setPlayerOne(usersID.at(0) == my_id);
  _their_username = _player.isPlayerOne() ? _client->GetUsername(usersID.at(1)).get() : _client->GetUsername(usersID.at(0)).get();
}

bool LocalBoardCommander::myTurn() const { return _player.isTurn(); }

bool LocalBoardCommander::isFinished() const { return _is_finished; }
bool LocalBoardCommander::isVictory() const { return _is_victory; }

std::size_t LocalBoardCommander::width() const {
  return _my_board.at(0).size();
}
std::size_t LocalBoardCommander::height() const { return _my_board.size(); }

GameMode LocalBoardCommander::mode() const { return _mode; }
Player LocalBoardCommander::player() const { return _player; }

void LocalBoardCommander::setPlayerFaction(Faction faction) {
  _player.setFaction(faction);
}

CellType LocalBoardCommander::cellType(bool my_side,
                                       BoardCoordinates coordinates) const {
  return get(my_side, coordinates).type();
}

bool LocalBoardCommander::isSameShip(bool my_side, BoardCoordinates first,
                                     BoardCoordinates second) const {
  if (get(true, first).type() != UNDAMAGED_SHIP ||
      get(true, second).type() != UNDAMAGED_SHIP) {
    return false;
  }
  return get(true, first).ship() == get(true, second).ship();
}

std::vector<Cell>
LocalBoardCommander::getNeighbors(BoardCoordinates coord) const {
  std::vector<Cell> neighbors;
  if (coord.x() > 0) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x() - 1, coord.y())));
  }
  if (coord.x() < width() - 1) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x() + 1, coord.y())));
  }
  if (coord.y() > 0) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x(), coord.y() - 1)));
  }
  if (coord.y() < height() - 1) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x(), coord.y() + 1)));
  }
  if (coord.x() > 0 && coord.y() > 0) {
    neighbors.push_back(
        get(true, BoardCoordinates(coord.x() - 1, coord.y() - 1)));
  }
  if (coord.x() > 0 && coord.y() < height() - 1) {
    neighbors.push_back(
        get(true, BoardCoordinates(coord.x() - 1, coord.y() + 1)));
  }
  if (coord.x() < width() - 1 && coord.y() > 0) {
    neighbors.push_back(
        get(true, BoardCoordinates(coord.x() + 1, coord.y() - 1)));
  }
  if (coord.x() < width() - 1 && coord.y() < height() - 1) {
    neighbors.push_back(
        get(true, BoardCoordinates(coord.x() + 1, coord.y() + 1)));
  }
  return neighbors;
}

std::vector<Ship> LocalBoardCommander::getPlacedShips() const {
  return _player.getFleet();
}

bool LocalBoardCommander::isShipAvailable(int size) const {

  int count = 0;

  for (auto &ship : _player.getFleet()) { // counts how many ships of the given
                                          // size are already placed
    if (ship.getLength() == size) {
      count++;
    }
  }

  // if the number of ships of the given size is less than the number of ships
  // of that size that can be placed, return true
  if (count < _player.getFaction().getPossibleShips()[size]) {
    return true;
  } else {
    return false;
  }
}

void LocalBoardCommander::placeShip(Ship ship) {
  BoardCoordinates top_left = ship.getTopLeft();

  for (auto &coord : ship.getCoordinates()) {
    _my_board.at(top_left.y() + coord.y())
        .at(top_left.x() + coord.x())
        .setType(ship.getType());
    _my_board.at(top_left.y() + coord.y())
        .at(top_left.x() + coord.x())
        .setShip(ship);
  }
  _player.addShip(ship);

  // if all ships are placed, send the ships to the server
  if (allShipsPlaced()) {
    nlohmann::json move_request;

    move_request["moveType"] = "placeShip"; //< should be "move" ?
    move_request["ships"] = nlohmann::json::array();
    for (auto &ship : _player.getFleet()) {
      move_request["ships"].push_back(ship.to_json());
    }
    std::cerr << move_request.dump() << std::endl;
    _client->MakeMove(_session_id, move_request);
  }
}

bool LocalBoardCommander::allShipsPlaced() const {
  for (auto &ship : shipsToPlace()) {
    if (ship.second > 0) {
      return false;
    }
  }
  return true;
}

PossibleShips LocalBoardCommander::shipsToPlace() const {
  PossibleShips placed_ships = _player.getFaction().getPossibleShips();
  for (auto &ship : _player.getFleet()) {
    placed_ships[ship.getLength()]--;
  }
  return placed_ships;
}

CellType LocalBoardCommander::best(CellType lhs, CellType rhs) {
  if (!(lhs & UNDAMAGED_SHIP) || !(rhs & UNDAMAGED_SHIP)) {
    std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
              << static_cast<unsigned>(rhs) << ")" << std::endl;
    throw std::logic_error("BoardView::best called with non-ship types");
  }
  return lhs <= rhs ? lhs : rhs;
}

bool LocalBoardCommander::waitGame() {
  bool shipPlacementsFinished = false;
  std::string turn;
  while (!shipPlacementsFinished) {
    shipPlacementsFinished = isGameStarted();
    sleep(1);
  }
  return fetchMyTurn();
}

bool LocalBoardCommander::isGameStarted() {
  auto FutureGameState = _client->QueryGameState(_session_id);
  auto result = FutureGameState.get();
  auto gameState = result.at("gameState");
  return gameState.at("_ship_placements_finished") == "true";
}

void LocalBoardCommander::waitTurn() {
  bool my_turn = false;
  while (!my_turn) {
    my_turn = fetchMyTurn();
    sleep(1);
  }
}

bool LocalBoardCommander::fetchMyTurn() {
  auto FutureGameState = _client->QueryGameState(_session_id);
  auto result = FutureGameState.get();
  auto gameState = result["gameState"];
  return gameState["turn"] == "PLAYERONE" && _player.isPlayerOne() ||
         gameState["turn"] == "PLAYERTWO" && !_player.isPlayerOne();
}

Cell LocalBoardCommander::get(bool my_side, BoardCoordinates position) const {
  return my_side ? _my_board.at(position.y()).at(position.x())
                 : _their_board.at(position.y()).at(position.x());
}

// Ship &LocalBoardCommander::shipId(bool my_side, BoardCoordinates position) {}

// bool LocalBoardCommander::check() {}

// void LocalBoardCommander::placeShip(ShipCoordinates coordinates, bool
// my_fleet) {}

// void LocalBoardCommander::fire(BoardCoordinates coordinates) {

// }

CellType LocalBoardCommander::string_to_celltype(const std::string &type) {
  if (type == "WATER") {
    return WATER;
  } else if (type == "OCEAN") {
    return OCEAN;
  } else if (type == "UNDAMAGED_MINE") {
    return UNDAMAGED_MINE;
  } else if (type == "SCANNED_MINE") {
    return SCANNED_MINE;
  } else if (type == "HIT_MINE") {
    return HIT_MINE;
  } else if (type == "UNDAMAGED_SHIP") {
    return UNDAMAGED_SHIP;
  } else if (type == "SCANNED_SHIP") {
    return SCANNED_SHIP;
  } else if (type == "HIT_SHIP") {
    return HIT_SHIP;
  } else if (type == "SUNK_SHIP") {
    return SUNK_SHIP;
  } else {
    throw std::runtime_error("Unknown CellType: " + type);
  }
}

void LocalBoardCommander::update_board(const nlohmann::json &new_board) {
  auto fleetA = new_board["fleetA"];
  auto fleetB = new_board["fleetB"];

  if (!(fleetA.is_string() && fleetA.get<std::string>() == "None")) {
    for (int i = 0; i < _my_board.size(); i++) {
      for (int j = 0; j < _my_board.at(0).size(); j++) {
        _my_board[i][j].setType(string_to_celltype(fleetA[i][j]["type"]));
      }
    }
  }
  if (!(fleetB.is_string() && fleetB.get<std::string>() == "None")) {
    for (int i = 0; i < _their_board.size(); i++) {
      for (int j = 0; j < _their_board.at(0).size(); j++) {
        _their_board[i][j].setType(string_to_celltype(fleetB[i][j]["type"]));
      }
    }
  }

  if (new_board["Finished"] == "true"){
    _is_finished = true;
    if((new_board["Winner"] == "PLAYERONE" && _player.isPlayerOne()) || (new_board["Winner"] == "PLAYERTWO" && !_player.isPlayerOne())){
      _is_victory = true;
    }else{
      _is_victory = false;
    }
  }else{
    _is_finished = false;
  }



}

bool LocalBoardCommander::isInBoard(BoardCoordinates coord) const {
  return coord.x() >= 0 && coord.x() < 10 && coord.y() >= 0 && coord.y() < 10;
}

void LocalBoardCommander::fire(SpecialAbility ability,
                               BoardCoordinates coordinates) {
  nlohmann::json move_request;
  nlohmann::json fire_request;

  fire_request["ability"] = ability.getType();
  fire_request["anchor"] = coordinates.to_json();

  move_request["moveType"] = "fire";
  move_request["fire"] = fire_request;

  _client->MakeMove(_game_id, move_request);
}

std::string LocalBoardCommander::getMyUsername() const { return _my_username; }

std::string LocalBoardCommander::getTheirUsername() const {
  return _their_username;
}