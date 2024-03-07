#include "game_controller.hh"
#include "cell_type.hh"
#include "board_coordinates.hh"
#include "ship_coordinates.hh"

GameController::GameController(std::shared_ptr<LocalBoard> board) : _board{std::move(board)} {}

bool GameController::fire(BoardCoordinates coord) const {
    // Sends POST request to fire to the gameServer
    return true;
}

bool GameController::checkShipsInBoard(ShipCoordinates coord) const {
    std::array<std::pair<ShipType, uint8_t>, 4> ships = _board->shipsToPlace();
    return ships.at(coord.ship_id() - 2).second > 0;
}

bool GameController::checkShipPosition(ShipCoordinates coord) const {
    for (int i = 0; i < coord.ship_id(); i++) {
        if (coord.orientation() == HORIZONTAL) {
            if (coord.x() + i < _board->width() && _board->cellType(true, BoardCoordinates(coord.x() + i, coord.y())) != UNDAMAGED) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x() + i, coord.y()))) {
                    if (neighbor.type() == IS_SHIP) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        } else {
            if (coord.y() + i < _board->height() && _board->cellType(true, BoardCoordinates(coord.x(), coord.y() + i)) != UNDAMAGED) {
                for (auto &neighbor: _board->getNeighbors(BoardCoordinates(coord.x(), coord.y() + i))) {
                    if (neighbor.type() == IS_SHIP) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
    }
    return true;
}

bool GameController::placeShip(ShipCoordinates coord) const {
    // Verifier qu'on peut poser le bateau la
    if ( checkShipsInBoard(coord) && checkShipPosition(coord)) {
        // Sends a request to place the ship to the gameServer
        _board->addPlacedShip(coord);
        return true;
    } else {
        return false;
    }
}

void GameController::sendShips(std::vector<ShipCoordinates> boats) {
    // POST request to the server to place the boats on the board
}

void GameController::quit() {}

void GameController::connectServer() {}

bool GameController::sendRequest(ShipCoordinates coord) {}
bool GameController::sendRequest(BoardCoordinates coord) {}