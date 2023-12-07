#include "board_control.hh"
#include "board_view.hh"


bool BoardControl::fire(BoardCoordinates coord) {
    _board->fire(coord);
    _board->changeTurn();
    _display->printChangeTurn();
}

bool BoardControl::checkShipPosition(ShipCoordinates coord) {
    bool isValid = true;
    for (int i = 0; i < coord.getLength() && isValid; i++) {
        if (coord.getVertical()) {
            if (coord.x() + i < _board->width() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x() + i, coord.y())) != BoardView::IS_SHIP) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x() + i, coord.y()))) {
                    if (_board->cellType(_board->myTurn(), BoardCoordinates(coord.x() + i, coord.y())) == BoardView::IS_SHIP) {
                        isValid = false;
                    }
                }
            } else {
                isValid = false;
            }
        } else {
            if (coord.y() + i < _board->height() && _board->cellType(_board->myTurn(), BoardCoordinates(coord.x(), coord.y() + i)) != BoardView::IS_SHIP) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x(), coord.y() + i))) {
                    if (_board->cellType(_board->myTurn(), BoardCoordinates(coord.x(), coord.y() + i)) == BoardView::IS_SHIP) {
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