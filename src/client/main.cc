#include <iostream>
#include <memory>

#include "console_board_display.hh"
#include "dummy_board.hh"

int main() {
  // Create the fake game (for testing purposes)
  std::shared_ptr<DummyBoard>          board = std::make_shared<DummyBoard>();

  // Create the display and conect it to the game
  std::shared_ptr<ConsoleBoardDisplay> display =
      std::make_shared<ConsoleBoardDisplay>(std::cout, std::cin, board, board);

  // Connect the game to the display
  board->setDisplay(display);

  // Run the game
  while (!board->isFinished()) {
    display->handleInput();
  }
}
