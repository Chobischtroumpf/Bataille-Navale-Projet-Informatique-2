#include "game_controller.hh"
#include "cell_type.hh"
#include "board_coordinates.hh"
#include "ship_coordinates.hh"

GameController::GameController(std::shared_ptr<LocalBoardCommander> board) : _board{std::move(board)} {}

bool GameController::fire(BoardCoordinates coord) const {
    // Sends POST request to fire to the gameServer
    return true;
}

bool GameController::checkShipPosition(Ship ship) const {
    for (auto coord : ship.getCoordinates()) {
        if (_board->getCell(coord) != CellType::WATER || !_board->isInBoard(coord))
            return false;
    }
    return true;
}

bool GameController::placeShip(ShipCoordinates coord) const {
    // Verifier qu'on peut poser le bateau la
    if ( checkShipPosition(coord)) {
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