#include "board_control.hh"
#include "board_view.hh"

bool BoardControl::fire(BoardCoordinates coord) {
    _board->fire(coord);
    _board->changeTurn();
    return true;
}

bool BoardControl::_checkShipsInBoard(ShipCoordinates coord) {
    map<ShipType, int> ships = _board->countShips(_board->myTurn());
    switch (ships[coord.ship_id()])
    {
    case 0:
        return true;
    case 1:
        if (coord.ship_id() == CARRIER || coord.ship_id() == SUBMARINE || coord.ship_id() == CRUISER) {
            return false;
        } else {
            return true;
        }
    case 2:
        if (coord.ship_id() == CARRIER) {
            return false;
        } else {
            return true;
        }
    default:
        return false;
    }
}

bool BoardControl::_checkShipPosition(ShipCoordinates coord) {
    bool isValid = true;
    try {
    for (int i = 0; i < coord.ship_id() && isValid; i++) {
        if (coord.orientation() == HORIZONTAL) {
            if (coord.x() + i < _board->width() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x() + i, coord.y())) != UNDAMAGED) {
                vector<Cell> neighbors = _board->getNeighbors(BoardCoordinates(coord.x() + i, coord.y()));
                for (auto &neighbor: neighbors) {
                    if (neighbor.type() == UNDAMAGED) {
                        std::cerr << "Invalid horizontal position" << std::endl;
                        isValid = false;
                    }
                }
            } else {
                std::cerr << "Invalid horizontal position2" << std::endl;
                isValid = false;
            }
        } else {
            if (coord.y() + i < _board->height() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x(), coord.y() + i)) != UNDAMAGED) {
                vector<Cell> neighbors = _board->getNeighbors(BoardCoordinates(coord.x(), coord.y() + i));
                for (auto &neighbor: neighbors) {
                    if (neighbor.type() == UNDAMAGED) {
                        std::cerr << "Invalid vertical position" << std::endl;
                        isValid = false;
                    }
                }
            } else {
                std::cerr << "Invalid vertical position2" << std::endl;
                isValid = false;
            }
        }
    }
    } catch (const std::exception& e) {
        isValid = false;
    }
    return isValid;
}

bool BoardControl::placeShip(ShipCoordinates coord) {
    if (coord.ship_id() != ShipType::NONE && _checkShipsInBoard(coord) && _checkShipPosition(coord)) {
        _board->placeShip(coord, _board->myTurn());
        _board->changeTurn();
        return true;
    } else {
        return false;
    }
}

void BoardControl::quit() {}