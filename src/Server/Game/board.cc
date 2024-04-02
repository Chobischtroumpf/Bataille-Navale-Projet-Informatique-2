#include "board.hh"

void Board::setHit(BoardCoordinates coords) {
  Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                      : _player1_side[coords.y()][coords.x()];
  Player &current_player = _my_turn ? _player1 : _player2;
  Player &opponent = _my_turn ? _player2 : _player1;

  if (cell.type() == UNDAMAGED_SHIP) {
    cell.setType(HIT_SHIP);
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
}

void Board::fireBigTorpedo(BoardCoordinates coords) {
  for (BigTorpedoIterator it = beginBigTorpedo(coords);
    it != endBigTorpedo(coords); ++it) {
    if (isInBoard(*it)){
        setHit(*it);
    }
  }
}

void Board::firePiercingTorpedo(BoardCoordinates coords) {
  for (PiercingTorpedoIterator it = beginPiercingTorpedo(coords); it != endPiercingTorpedo(coords); ++it) {
    if (isInBoard(*it)){
      setHit(*it);
    }
  }
}

void Board::fireAerialStrike(BoardCoordinates coords) {
  for (AerialStrikeIterator it = beginAerialStrike(coords); it != endAerialStrike(coords); ++it) {
    if (isInBoard(*it)) {
      setHit(*it);
    }
  }
}

void Board::dispatchTorpedo(SpecialAbilityType ability_type, BoardCoordinates coords) {
  if (ability_type == TORPEDO)
    setHit(coords);
  else if (ability_type == BIG_TORPEDO)
    fireBigTorpedo(coords);
  else if (ability_type == PIERCING_TORPEDO)
    firePiercingTorpedo(coords);
  else if (ability_type == AERIAL_STRIKE)
    fireAerialStrike(coords);
}

void Board::setScanned(BoardCoordinates coords) {
  Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                        : _player1_side[coords.y()][coords.x()];
  if (cell.type() == UNDAMAGED_SHIP) {
    cell.setType(SCANNED_SHIP);
  } else if (cell.type() == UNDAMAGED_MINE) {
    cell.setType(SCANNED_MINE);
  } else if (cell.type() == WATER) {
    cell.setType(SCANNED);
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

void Board::dispatchSonar(SpecialAbilityType ability_type, BoardCoordinates coords) {
  if (ability_type == SONAR)
    fireSonar(coords);
  else if (ability_type == PROBE)
    fireProbe(coords);
}

void Board::setMine(SpecialAbility ability, BoardCoordinates coords) {
  Player& current_player = _my_turn ? _player1 : _player2;
  current_player.addShip(Ship(coords, {{0,0}}, this));
}

Board::Board(): _my_turn{true}, _player1{}, _player2{}, _fleetA_state{true}, _fleetB_state{true},
                _player1_side{10, std::vector<Cell>{10, {}}}, _player2_side{10, std::vector<Cell>{10, {}}} {
}

Board::Board(Player player1, Player player2): _my_turn{true}, _player1{player1}, _player2{player2},
      _fleetA_state{true}, _fleetB_state{true}, _player1_side{10, std::vector<Cell>{10, {}}},
       _player2_side{10, std::vector<Cell>{10, {}}} {
}

size_t Board::width() const {
  return _player1_side.at(0).size();
}

size_t Board::height() const {
  return _player1_side.size();
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

void Board::placeShip(Ship ship, bool side) {
  std::vector<std::vector<Cell>> &board = side ? _player1_side : _player2_side;
  Player &player = side ? _player1 : _player2;
  for (auto& board_coordinates : ship.getCoordinates()){
    board[ship.getTopLeft().y() + board_coordinates.y()][ship.getTopLeft().x() + board_coordinates.x()].setType(ship.getType());
  }
  player.addShip(ship);
}

void Board::notify(const BoardCoordinates &coords) {
  // Iterate over the enemy's ships
  Player& current_player = _my_turn ? _player2 : _player1;

  for (Ship &ship : current_player.getFleet()) {
    ship.notify(coords);
    if (ship.getType() != UNDAMAGED_MINE && !ship.isSunk()) {
      return;
    }
  }
  if (_my_turn){
    _fleetB_state = false;
  } else{
    _fleetA_state = false;
  }
}

void Board::fire(SpecialAbility ability, BoardCoordinates coords) {
  Player& current_player = _my_turn ? _player1 : _player2;

  current_player.removeEnergyPoints(ability.getEnergyCost());
  
  if (ability.getType() & IS_TORPEDO) {
    dispatchTorpedo(ability.getType(), coords);
  } else if (ability.getType() & IS_SONAR) {
    fireSonar(coords);
  } else if (ability.getType() & MINE) {
    setMine(ability, coords);        
  }
  notify(coords);
}

// converts a CellType to a string
static string Board::to_string(CellType type) {
  switch (type) {
  case WATER:
    return "WATER";
  case OCEAN:
    return "OCEAN";
  case SCANNED:
    return "SCANNED";
  case UNDAMAGED_MINE:
    return "UNDAMAGED_MINE";
  case SCANNED_MINE:
    return "SCANNED_MINE";
  case HIT_MINE:
    return "HIT_MINE";
  case UNDAMAGED_SHIP:
    return "UNDAMAGED_SHIP";
  case SCANNED_SHIP:
    return "SCANNED_SHIP";
  case HIT_SHIP:
    return "HIT_SHIP";
  case SUNK_SHIP:
    return "SUNK_SHIP";
  default:
    throw NotImplementedError("Board unknown CellType");
  }
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
bool Board::isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const  override {
  return shipId(my_side, first).has_value() &&
        shipId(my_side, first) == shipId(my_side, second);
}

bool Board::isInBoard(BoardCoordinates coord) const {
  return coord.x() >= 0 && coord.x() < width() && coord.y() >= 0 && coord.y() < height();
}

// Iterators methods
// Big Torpedo
BigTorpedoIterator Board::beginBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords, width(), height());
}

BigTorpedoIterator Board::endBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords + BoardCoordinates(2, 0), width(), height());
}

// Piercing Torpedo
PiercingTorpedoIterator Board::beginPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords, width(), height());
}

PiercingTorpedoIterator Board::endPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords + BoardCoordinates(0, 4), width(), height());
}

// Aerial Strike
AerialStrikeIterator Board::beginAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords, width(), height());
}

AerialStrikeIterator Board::endAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords + BoardCoordinates(4, 3), width(), height());
}