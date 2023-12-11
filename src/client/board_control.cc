#include "board_control.hh"
#include "board_view.hh"


bool BoardControl::fire(BoardCoordinates coord) {
    _board->fire(coord);
    _board->changeTurn();
    return true;
}

bool BoardControl::checkShipPosition(ShipCoordinates coord) {
    bool isValid = true;
    for (int i = 0; i < coord.ship_id() && isValid; i++) {
        if (coord.orientation() == HORIZONTAL) {
            if (coord.x() + i < _board->width() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x() + i, coord.y())) != UNDAMAGED) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x() + i, coord.y()))) {
                    if (_board->cellType(_board->myTurn(), BoardCoordinates(coord.x() + i, coord.y())) == UNDAMAGED) {
                        isValid = false;
                    }
                }
            } else {
                isValid = false;
            }
        } else {
            if (coord.y() + i < _board->height() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x(), coord.y() + i)) != UNDAMAGED) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x(), coord.y() + i))) {
                    if (_board->cellType(_board->myTurn(), BoardCoordinates(coord.x(), coord.y() + i)) == UNDAMAGED) {
                        isValid = false;
                    }
                }
            } else {
                isValid = false;
            }
        }
    }
    return isValid;
}

bool BoardControl::placeShip(ShipCoordinates coord) {
    // Verifier qu'on peut poser le bateau la
    if (checkShipPosition(coord)) {
        _board->placeShip(coord);
        _board->changeTurn();
        return true;
    } else {
        return false;
    }
}

void BoardControl::quit() {}