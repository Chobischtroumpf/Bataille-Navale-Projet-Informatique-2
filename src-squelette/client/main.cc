#include "Controllers/game_controller.hh"
#include "Display/Console/game_console.hh"
#include "local_board.hh"
#include <iostream>
#include <memory>

// #include "dummy_board.hh"

int main() {
    std::shared_ptr<LocalBoard> local_board = std::make_shared<LocalBoard>();
    std::shared_ptr<GameController> controller = std::make_shared<GameController>(local_board);
    std::shared_ptr<GameConsole> console = std::make_shared<GameConsole>(std::cout, std::cin, local_board, controller);
}
