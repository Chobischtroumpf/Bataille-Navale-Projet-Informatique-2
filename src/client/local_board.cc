#include "../../include/client/local_board.hh"
#include <vector>
#include <unistd.h>
#include <iostream>


LocalBoard::LocalBoard()
    : _is_finished{false},
      _is_victory{false},
      _my_turn{true},
      _my_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))},
      _their_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))} {}

bool LocalBoard::myTurn() const { return _my_turn; }

bool LocalBoard::isFinished() const { return _is_finished; }
bool LocalBoard::isVictory() const { return _is_victory; }

std::size_t LocalBoard::width() const { return _my_board.at(0).size(); }
std::size_t LocalBoard::height() const { return _my_board.size(); }

CellType LocalBoard::cellType(bool my_side, BoardCoordinates coordinates) const {
  return get(my_side, coordinates).type();
}

bool LocalBoard::isSameShip(bool my_side, BoardCoordinates first,
                            BoardCoordinates second) const {
  if (get(my_side, first).type() != IS_SHIP || get(my_side, second).type() != IS_SHIP) {
    return false;
  }
  return get(my_side, first).shipId() == get(my_side, second).shipId();
}

std::vector<Cell> LocalBoard::getNeighbors(BoardCoordinates coord) const {
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

bool LocalBoard::isRemainingShip(ShipType ship_type) const {
  return _ships_to_place.at(ship_type - 2).second > 0;
}

std::vector<ShipCoordinates> LocalBoard::getPlacedShips() const {
  return _placed_ships;
}

void LocalBoard::addPlacedShip(ShipCoordinates coord) {
  _placed_ships.push_back(coord);
  isRemainingShip(coord.ship_id()) ? _ships_to_place.at(coord.ship_id() - 2).second-- : throw std::logic_error("Ship already placed");
  for (int i = 0; i < coord.ship_id(); i++) {
    BoardCoordinates new_coord = coord.orientation() == HORIZONTAL ? BoardCoordinates(coord.x() + i, coord.y()) : BoardCoordinates(coord.x(), coord.y() + i);
    _my_board.at(new_coord.y()).at(new_coord.x()).setType(IS_SHIP);
  }
}

bool LocalBoard::allBoatsPlaced() const {
  for (auto ship : _ships_to_place) {
    if (ship.second > 0) {
      return false;
    }
  }
  return true;
}

std::array<std::pair<ShipType, uint8_t>, 4> LocalBoard::shipsToPlace() const {
  return _ships_to_place;
}

constexpr inline CellType LocalBoard::best(CellType lhs, CellType rhs) {
  if (!(lhs & IS_SHIP) || !(rhs & IS_SHIP)) {
    std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
              << static_cast<unsigned>(rhs) << ")" << std::endl;
    throw std::logic_error("BoardView::best called with non-ship types");
  }
  return lhs <= rhs ? lhs : rhs;
}

void LocalBoard::waitGame() {
  // Wait for the game to start
}

Cell LocalBoard::get(bool my_side, BoardCoordinates position) const {
  return my_side ? _my_board.at(position.y()).at(position.x()) : _their_board.at(position.y()).at(position.x());
}

int LocalBoard::shipId(bool my_side, BoardCoordinates position) {}

bool LocalBoard::check() {}

void LocalBoard::placeShip(ShipCoordinates coordinates, bool my_fleet) {}

void LocalBoard::fire() { }
