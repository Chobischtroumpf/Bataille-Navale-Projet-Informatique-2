#include "game_controller.hh"
#include "cell_type.hh"
#include "board_coordinates.hh"
#include "local_board_commander.hh"

GameController::GameController(std::shared_ptr<LocalBoardCommander> board) : _board{std::move(board)} {}

bool GameController::fire(BoardCoordinates coord) const {
    // Sends POST request to fire to the gameServer
    return true;
}

bool GameController::checkShipPosition(Ship ship) const {
    for (auto coord : ship.getCoordinates()) {
        if (_board->cellType(true, coord) != CellType::WATER || !_board->isInBoard(coord))
            return false;
    }
    return true;
}

bool GameController::placeShip(Ship ship) const {
    // Verifier qu'on peut poser le bateau la
    if ( checkShipPosition(ship)) {
        // Sends a request to place the ship to the gameServer
        _board->addPlacedShip(ship);
        return true;
    } else {
        return false;
    }
}

void GameController::sendShips(std::vector<Ship> boats) {
    // POST request to the server to place the boats on the board
}

void GameController::quit() {}

void GameController::connectServer() {}

bool GameController::sendRequest(Ship ship) {return true;}
bool GameController::sendRequest(BoardCoordinates coord) {return true;}