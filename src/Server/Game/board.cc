#include "board.hh"
#include <iostream>

bool Board::setHit(BoardCoordinates coords) {
  Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                      : _player1_side[coords.y()][coords.x()];
  Player &current_player = _my_turn ? _player1 : _player2;
  Player &opponent = _my_turn ? _player2 : _player1;
  bool is_hit = false;

  if (cell.type() == UNDAMAGED_SHIP) {
    cell.setType(HIT_SHIP);
    is_hit = true;
  } else if (cell.type() == UNDAMAGED_MINE) {
    cell.setType(HIT_MINE);
    if (opponent.getFaction().getName() == "Mines"){
        current_player.removeEnergyPoints(3);
    } else {
        current_player.removeEnergyPoints(2);
    }
  } else if (cell.type() == WATER) {
    cell.setType(OCEAN);
  }

  return is_hit;
}

bool Board::fireBigTorpedo(BoardCoordinates coords) {
  bool is_hit = false;

  for (BigTorpedoIterator it = beginBigTorpedo(coords);
    it != endBigTorpedo(coords); ++it) {
    if (isInBoard(*it)){
        is_hit = setHit(*it);
    }
  }
  return is_hit;
}

bool Board::firePiercingTorpedo(BoardCoordinates coords) {
  bool is_hit = false;

  for (PiercingTorpedoIterator it = beginPiercingTorpedo(coords); it != endPiercingTorpedo(coords); ++it) {
    if (isInBoard(*it)){
      is_hit = setHit(*it);
    }
  }

  return is_hit;
}

bool Board::fireAerialStrike(BoardCoordinates coords) {
  bool is_hit = false;

  for (AerialStrikeIterator it = beginAerialStrike(coords); it != endAerialStrike(coords); ++it) {
    if (isInBoard(*it)) {
      is_hit = setHit(*it);
    }
  }

  return is_hit;
}

bool Board::dispatchTorpedo(SpecialAbilityType ability_type, BoardCoordinates coords) {
  bool is_hit = false;
  
  if (ability_type == TORPEDO)
    is_hit = setHit(coords);
  else if (ability_type == BIG_TORPEDO)
    is_hit = fireBigTorpedo(coords);
  else if (ability_type == PIERCING_TORPEDO)
    is_hit = firePiercingTorpedo(coords);
  else if (ability_type == AERIAL_STRIKE)
    is_hit = fireAerialStrike(coords);

  return is_hit;
}

void Board::setScanned(BoardCoordinates coords) {
  Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                        : _player1_side[coords.y()][coords.x()];
  if (cell.type() == UNDAMAGED_SHIP) {
    cell.setType(SCANNED_SHIP);
  } else if (cell.type() == UNDAMAGED_MINE) {
    cell.setType(SCANNED_MINE);
  } else if (cell.type() == WATER) {
    cell.setType(WATER);
  }
}

void Board::fireSonar(BoardCoordinates coords) {
  for (size_t i = 0; i < 3; i++){
    for (size_t j = 0; j < 3; j++){
      setScanned(BoardCoordinates(i,j)+coords);
    }
  }
}

void Board::fireProbe(BoardCoordinates coords) {
  for (int i = this->width(); i > 0; i--)
    setScanned(BoardCoordinates(i+1, coords.y()));
}

void Board::setMine(BoardCoordinates coords) {
  Player& current_player = _my_turn ? _player1 : _player2;
  current_player.addShip(Ship(coords, {{0,0}}, this, _my_turn));
}

void Board::dispatchSonar(SpecialAbilityType ability_type, BoardCoordinates coords) {
  if (ability_type == SONAR)
    fireSonar(coords);
  else if (ability_type == PROBE)
    fireProbe(coords);
}

Board::Board(): _my_turn{true}, _player1{}, _player2{}, _fleetA_state{true}, _fleetB_state{true},
                _player1_side{10, {10, Cell()}}, _player2_side{10, {10, Cell()}} {
}

Board::Board(Player player1, Player player2): _my_turn{true}, _player1{player1}, _player2{player2},
      _fleetA_state{true}, _fleetB_state{true}, _player1_side{10, {10, Cell()}},
       _player2_side{10, {10, Cell()}} {
}

size_t Board::width() const {
  return _player1_side.at(0).size();
}

size_t Board::height() const {
  return _player1_side.size();
}

bool Board::myTurn() const {
  return _my_turn;
}

Player& Board::getPlayer1() {
  return _player1;
}

Player& Board::getPlayer2() {
  return _player2;
}

void Board::changeTurn() {
  _my_turn = !_my_turn;
  _player1.swapTurn();
  _player2.swapTurn();
  if (_my_turn){
    _player1.addEnergyPoints(1);
  } else {
    _player2.addEnergyPoints(1);
  }
}

bool Board::isFinished() const {
  return !(_fleetA_state && _fleetB_state);
}

bool Board::isVictory() const {
  return _fleetA_state;
}

Turn Board::whoseTurn() const {
  if (_my_turn){
    return PLAYERONE;
  }else{
    return PLAYERTWO;
  }
}

void Board::placeShip(Ship &ship, bool side) {
  std::vector<std::vector<Cell>> &board = side ? _player1_side : _player2_side;
  Player &player = side ? _player1 : _player2;
  for (auto& board_coordinates : ship.getCoordinates()){
    board[ship.getTopLeft().y() + board_coordinates.y()][ship.getTopLeft().x() + board_coordinates.x()].setType(ship.getType());
  }
  player.addShip(ship);
}

void Board::notify(const BoardCoordinates &coords) {
  // Iterate over the targeted player's ships
  Player& current_player = _my_turn ? _player2 : _player1;

  for (Ship &ship : current_player.getFleet()) {
    std::clog << "isPartOfShip: " << ship.isPartOfShip(coords) << std::endl;
    std::clog << "isSunk: " << ship.isSunk() << std::endl;
    if (ship.isPartOfShip(coords) && !ship.isSunk()) {
      ship.notify();
      std::clog << "Ship is sunk: " << ship.isSunk() << std::endl;
      if (ship.isSunk() && !(ship.getType() & IS_MINE)) {
        std::vector<BoardCoordinates> ship_coords = ship.getCoordinates();
        BoardCoordinates top_left = ship.getTopLeft();
        for (auto &coord : ship_coords) {
          _player1_side[top_left.y() + coord.y()][top_left.x() + coord.x()].setType(SUNK_SHIP);
        }
      }
    }
    if (!(ship.getType() & IS_MINE) && !ship.isSunk()) {
      return;
    }
  }
  if (_my_turn){
    _fleetB_state = false;
  } else{
    _fleetA_state = false;
  }
}

bool Board::fire(SpecialAbility ability, BoardCoordinates coords) {
  Player& current_player = _my_turn ? _player1 : _player2;
  bool is_hit = false;

  current_player.removeEnergyPoints(ability.getEnergyCost());
  
  if (ability.getType() & IS_TORPEDO) {
    is_hit = dispatchTorpedo(ability.getType(), coords);
  } else if (ability.getType() & IS_SONAR) {
    dispatchSonar(ability.getType(), coords);
  } else if (ability.getType() & MINE) {
    setMine(coords);  
  }
  notify(coords);
  return is_hit;
}

// converts the board to a json object
nlohmann::json Board::to_json(PlayerRole role) const {
  nlohmann::json boardJson;
  nlohmann::json fleetAJson, fleetBJson;

  for (size_t y = 0; y < height(); ++y) {
    nlohmann::json rowJsonA, rowJsonB;

    for (size_t x = 0; x < width(); ++x) {
      nlohmann::json cellObjectA, cellObjectB;
      Cell cellA, cellB;

      if (role == PlayerRole::Leader || role == PlayerRole::Spectator) {
        cellA = _player1_side[y][x];
        cellB = _player2_side[y][x];
      } else {
        cellA = _player2_side[y][x];
        cellB = _player1_side[y][x];
      }

      // always sets the true type of the cell in cellObjectA as it will always be the player's side 
      cellObjectA["type"] = to_string(cellA.type());
      rowJsonA.push_back(cellObjectA);

      // if player is leader or opponent it will hide the type of the cell as it is not the player's side
      if ((role == PlayerRole::Leader || role == PlayerRole::Opponent) &&
          (cellB.type() == UNDAMAGED_SHIP || cellB.type() == UNDAMAGED_MINE)) {
        cellObjectB["type"] = to_string(WATER);
      } else {
        // if player is a spectator it will show the type of the cell
        cellObjectB["type"] = to_string(cellB.type());
      }
      rowJsonB.push_back(cellObjectB);
    }
    fleetAJson.push_back(rowJsonA);
    fleetBJson.push_back(rowJsonB);
  }

  boardJson["fleetA"] = fleetAJson;
  boardJson["fleetB"] = fleetBJson;

  if (role == PlayerRole::Leader) {
    boardJson["energy_points"] = _player1.getEnergyPoints();
  } else if (role == PlayerRole::Opponent) {
    boardJson["energy_points"] = _player2.getEnergyPoints();
  } else {
    boardJson["energy_points"] = 0;
  }

//  boardJson[""]
  

  return boardJson;
}

// returns the cell at the given position
Cell Board::get(bool my_side, BoardCoordinates position) const {
  if (my_side) {
    return _player1_side.at(position.y()).at(position.x());
  } else {
    return _player2_side.at(position.y()).at(position.x());
  }
}

// returns the type of the cell at the given position
CellType Board::cellType(bool my_side, BoardCoordinates position) const {
  return get(my_side, position).type();
}

// returns the ship at the given position
std::optional<Ship> Board::shipId(bool my_side, BoardCoordinates position) const {
  return get(my_side, position).ship();
}

// returns true if the two given positions are part of the same ship
bool Board::isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const {
  return shipId(my_side, first).has_value() &&
        shipId(my_side, first) == shipId(my_side, second);
}

bool Board::isInBoard(BoardCoordinates coord) const {
  return (coord.x() < width() && coord.y() < height());
}

// Iterators methods
// Big Torpedo
BigTorpedoIterator Board::beginBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords);
}

BigTorpedoIterator Board::endBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords + BoardCoordinates(2, 0));
}

// Piercing Torpedo
PiercingTorpedoIterator Board::beginPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords);
}

PiercingTorpedoIterator Board::endPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords + BoardCoordinates(0, 4));
}

// Aerial Strike
AerialStrikeIterator Board::beginAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords);
}

AerialStrikeIterator Board::endAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords + BoardCoordinates(4, 3));
}