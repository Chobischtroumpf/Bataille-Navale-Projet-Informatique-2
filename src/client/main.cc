#include "../../include/client/Controllers/game_controller.hh"
#include "../../include/client/Display/Console/game_console.hh"
#include "../../include/client/local_board.hh"
#include <iostream>
#include <memory>

// #include "dummy_board.hh"

#include "../../include/client/driver.hh" 

int main() {
    Driver app(DisplayType::CONSOLE); 

    // Lancement de l'application
    app.launchApp();

    return 0;
}