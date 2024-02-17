#include "game_controller.hh"
#include "../../common/cell_type.hh"
#include "../../common/board_coordinates.hh"
#include "../../common/ship_coordinates.hh"
#include <memory>
#include <map>


GameController::GameController(std::shared_ptr<LocalBoard> board) : _board{std::move(board)} {}

bool GameController::fire(BoardCoordinates coord) const {
    // Sends a request to fire to the gameServer
    return true;
}

bool GameController::checkShipsInBoard(ShipCoordinates coord) const {
    std::map<ShipType, uint8_t> ships = _board->countShips(_board->myTurn());
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

bool GameController::checkShipPosition(ShipCoordinates coord) const {
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

bool GameController::placeShip(ShipCoordinates coord) const {
    // Verifier qu'on peut poser le bateau la
    if ( checkShipsInBoard(coord) && checkShipPosition(coord)) {
        // Sends a request to place the ship to the gameServer
        return true;
    } else {
        return false;
    }
}

void GameController::quit() {}

void GameController::connectServer() {}

bool GameController::sendRequest(ShipCoordinates coord) {}
bool GameController::sendRequest(BoardCoordinates coord) {}