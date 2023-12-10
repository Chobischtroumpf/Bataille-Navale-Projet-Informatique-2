#pragma once

#include <memory>
#include "console_board_display.hh"
#include "board.hh"
#include "board_control.hh"

class GameDriver {
public:
    GameDriver();
    GameDriver(const GameDriver&)            = delete;
    GameDriver(GameDriver&&)                 = delete;
    GameDriver& operator=(const GameDriver&) = delete;
    GameDriver& operator=(GameDriver&&)      = delete;
    bool isFinished();
    void placeShips();
    void play();

private:
    std::shared_ptr<ConsoleBoardDisplay> _displayPlayer1;
    std::shared_ptr<ConsoleBoardDisplay> _displayPlayer2;
    std::shared_ptr<Board> _board;
    std::shared_ptr<BoardControl> _controller;
};