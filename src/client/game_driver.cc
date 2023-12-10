#include "game_driver.hh"


GameDriver::GameDriver() : _board{std::make_shared<Board>()}, _controller{std::make_shared<BoardControl>(_board)} {
    _displayPlayer1 = std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, _board, _controller);
    _displayPlayer2 = std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, _board, _controller);
}

void GameDriver::swapTurn() {
    _turn = !_turn;
}

bool GameDriver::isFinished() {
    return false;
}

void GameDriver::placeShips() {
    for (int i = 0; i < 4; i++) {
        if (_turn) {
            _displayPlayer1->update();
            _displayPlayer1->handlePlaceShip();
        } else {
            _displayPlayer2->update();
            _displayPlayer2->handlePlaceShip();
        }
        swapTurn();
    }
}

void GameDriver::play() {
    while (!isFinished()) {
        placeShips();
        if (_turn) {
            _displayPlayer1->update();
            _displayPlayer1->handleFire();
        } else {
            _displayPlayer2->update();
            _displayPlayer2->handleFire();
        }

    }
    swapTurn();
}