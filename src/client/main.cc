#include <iostream>
#include <memory>

#include "console_board_display.hh"
#include "console_board_gen_display.hh"
// #include "dummy_board.hh"
#include "board_control.hh"
#include "board.hh"

int main() {

  std::shared_ptr<Board>          board = std::make_shared<Board>();
  std::shared_ptr<BoardControl> controller = std::make_shared<BoardControl>(board);

  // Create the display and connect it to the game
  std::shared_ptr<ConsoleBoardDisplay> display =
            std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, board, controller);

  // Connect the game to the display
  board->setDisplay(display);
  controller->setDisplay(display);

  // Run the game
  while (!board->isFinished()) {
    display->handleInput();
  }
}
