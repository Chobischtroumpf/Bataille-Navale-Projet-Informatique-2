#include "local_board_commander.hh"
// #include <unistd.h>
// #include <iostream>


LocalBoardCommander::LocalBoardCommander(Player player)
    : _player{player},
      _is_finished{false},
      _is_victory{false},
      _my_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))},
      _their_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))} {}

bool LocalBoardCommander::myTurn() const { return _my_turn; }

bool LocalBoardCommander::isFinished() const { return _is_finished; }
bool LocalBoardCommander::isVictory() const { return _is_victory; }

std::size_t LocalBoardCommander::width() const { return _my_board.at(0).size(); }
std::size_t LocalBoardCommander::height() const { return _my_board.size(); }

CellType LocalBoardCommander::cellType(bool my_side, BoardCoordinates coordinates) const {
  return get(my_side, coordinates).type();
}

bool LocalBoardCommander::isSameShip(bool my_side, BoardCoordinates first,
                            BoardCoordinates second) const {
  if (get(my_side, first).type() != IS_SHIP || get(my_side, second).type() != IS_SHIP) {
    return false;
  }
  return get(my_side, first).shipId() == get(my_side, second).shipId();
}

std::vector<Cell> LocalBoardCommander::getNeighbors(BoardCoordinates coord) const {
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
    neighbors.push_back(get(true, BoardCoordinates(coord.x() - 1, coord.y() - 1)));
  }
  if (coord.x() > 0 && coord.y() < height() - 1) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x() - 1, coord.y() + 1)));
  }
  if (coord.x() < width() - 1 && coord.y() > 0) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x() + 1, coord.y() - 1)));
  }
  if (coord.x() < width() - 1 && coord.y() < height() - 1) {
    neighbors.push_back(get(true, BoardCoordinates(coord.x() + 1, coord.y() + 1)));
  }
  return neighbors;
}

bool LocalBoardCommander::isRemainingShip(int number_of_case) const {
  return _ships_to_place.at(number_of_case - 1) > 0;
}

std::vector<Ship> LocalBoardCommander::getPlacedShips() const {
  return _placed_ships;
}

void LocalBoardCommander::addPlacedShip(Ship ship, int x, int y) {
  _placed_ships.push_back(ship);
  isRemainingShip(ship.getNumberOfCase()) ? _ships_to_place.at(ship.getNumberOfCase())-- : throw std::logic_error("Ship already placed");
  for (auto &c : ship.getCoordinates()) {
    _my_board.at(y + c.y()).at(x + c.x()).setType(IS_SHIP);
  }
}

bool LocalBoardCommander::allBoatsPlaced() const {
  for (auto ship : _ships_to_place) {
    if (ship > 0) {
      return false;
    }
  }
  return true;
}

std::array<uint8_t, 5> LocalBoardCommander::shipsToPlace() const {
  return _ships_to_place;
}

CellType LocalBoardCommander::best(CellType lhs, CellType rhs) {
  if (!(lhs & IS_SHIP) || !(rhs & IS_SHIP)) {
    std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
              << static_cast<unsigned>(rhs) << ")" << std::endl;
    throw std::logic_error("BoardView::best called with non-ship types");
  }
  return lhs <= rhs ? lhs : rhs;
}

void LocalBoardCommander::waitGame() {
  // Wait for the game to start
}

Cell LocalBoardCommander::get(bool my_side, BoardCoordinates position) const {
  return my_side ? _my_board.at(position.y()).at(position.x()) : _their_board.at(position.y()).at(position.x());
}

int LocalBoardCommander::shipId(bool my_side, BoardCoordinates position) {}

bool LocalBoardCommander::check() {}

//void LocalBoardCommander::placeShip(ShipCoordinates coordinates, bool my_fleet) {}

void LocalBoardCommander::fire() { }
