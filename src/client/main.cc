#include <iostream>
#include <memory>

#include "console_board_display.hh"
// #include "dummy_board.hh"
#include "board_control.hh"
#include "board.hh"
#include "game_driver.hh"

int main() {
    std::shared_ptr<GameDriver> game = std::make_shared<GameDriver>();
    game->play();

}
