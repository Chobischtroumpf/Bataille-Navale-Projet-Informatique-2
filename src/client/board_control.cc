#include "board_control.hh"
#include "board_view.hh"


bool BoardControl::fire(BoardCoordinates coord) {
    _board->fire(coord);
    _board->changeTurn();
    _display->printChangeTurn();
}

bool BoardControl::checkShipPosition(ShipCoordinates coord) {
    bool isValid = true;
    for (int i = 0; i < coord.length && isValid; i++) {
        if (coord.vertical) {
            if (coord.anchor.x() + i < _board->width() && _board->CellType(_board->myTurn(), BoardCoordinates(coord.anchor.x() + i, coord.anchor.y())) != BoardView::CellType::IS_SHIP) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.anchor.x() + i, coord.anchor.y()))) {
                    if (_board->CellType(_board->myTurn(), BoardCoordinates(coord.anchor.x() + i, coord.anchor.y())) == 0b001) {
                        isValid = false;
                    }
                }
            } else {
                isValid = false;
            }
        } else {
            if (coord.anchor.y() + i < _board->height() && _board->CellType(_board->myTurn(), BoardCoordinates(coord.anchor.x(), coord.anchor.y() + i)) != IS_SHIP) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.anchor.x(), coord.anchor.y() + i))) {
                    if (_board->CellType(_board->myTurn(), BoardCoordinates(coord.anchor.x(), coord.anchor.y() + i)) == 0b001) {
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
        _display->printChangeTurn();
        return true;
    } else {
        return false;
    }
}

void BoardControl::quit() {}