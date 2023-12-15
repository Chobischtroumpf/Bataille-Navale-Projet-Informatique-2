#include "game_driver.hh"
#include "turn.hh"


GameDriver::GameDriver() : _board{std::make_shared<Board>()}, _controller{std::make_shared<BoardControl>(_board)} {
    _displayPlayer1 = std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, _board, _controller, PLAYERONE);
    _displayPlayer2 = std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, _board, _controller, PLAYERTWO);
}

bool GameDriver::isFinished() {
    return _board->isFinished();
}

void GameDriver::placeShips() {
    for (int i = 0; i < 10; i++) {
        if (_board->whoseTurn() == PLAYERONE) {
            _displayPlayer1->updatePlaceShip();
            _displayPlayer1->handlePlaceShip();
        } else {
            _displayPlayer2->updatePlaceShip();
            _displayPlayer2->handlePlaceShip();
        }
    }
}

void GameDriver::play() {
    placeShips();
    while (!isFinished()) {
        if (_board->whoseTurn() == PLAYERONE) {
            _displayPlayer1->updateGame();
            _displayPlayer1->handleFire();
        } else {
            _displayPlayer2->updateGame();
            _displayPlayer2->handleFire();
        }
    }
}