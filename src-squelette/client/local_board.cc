#include "local_board.hh"



std::map<ShipType, uint8_t> LocalBoard::countShips(bool my_side) const {
}

std::vector<Cell> LocalBoard::getNeighbors(BoardCoordinates coord) const {
}

Cell LocalBoard::get(bool my_side, BoardCoordinates position) {
}

int LocalBoard::shipId(bool my_side, BoardCoordinates position) {
}

bool LocalBoard::check() {
}

void LocalBoard::placeShip(ShipCoordinates coordinates, bool my_fleet) {
}

void LocalBoard::fire() {
}

bool LocalBoard::isSameShip(bool my_side, BoardCoordinates first,
                            BoardCoordinates second) {
}