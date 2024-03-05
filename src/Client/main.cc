#include <iostream>
#include <memory>

#include "game_controller.hh"
#include "game_console.hh"
#include "local_board.hh"
#include "driver.hh" 


int main() {
    Driver app(CONSOLE); 

    // Lancement de l'application
    app.launchApp();

    return 0;
}