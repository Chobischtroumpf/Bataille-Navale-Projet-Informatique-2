#include "board_control.hh"

bool BoardControl::fire(BoardCoordinates coord) {
    _board->fire(coord);
    _board->changeTurn();
    _display->printChangeTurn();
}

void BoardControl::quit() {}