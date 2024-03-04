#include "../../../../include/client/Display/Console/game_player_console.hh"

GamePlayerConsole::GamePlayerConsole(std::shared_ptr<GamePlayerView> view): _view(view) {}

void GamePlayerConsole::displayTitle() {
    std::cout << "╔═══════════════════════════════╗" << std::endl;
    std::cout << "║ Select the player of the game ║" << std::endl;
    std::cout << "╠═══════════════════════════════╩═══════════════════════════╪\n║" << std::endl;
}

void GamePlayerConsole::displayPlayer() {
    displayTitle();
    int i = 1;

    for (auto name : _view->getPlayerName()) {
        std::cout << "║ (" << i << ") - " << "\033[0;33m" << name << "\033[0m" << std::endl;
        i++;
    }

    for (int j = i; j < 10; j++) { std::cout << "║ (" << j << ") -" << std::endl; }
}

void GamePlayerConsole::displayPlayer(int p1) {
    displayTitle();
    int i = 1;

    for (auto name : _view->getPlayerName()) {
        std::cout << "║ (" << i << ") - ";
        if (i == p1) { std::cout << "\033[0;34m" << "*player 1* " << "\033[0m"; }
        std::cout << "\033[0;33m" << name << "\033[0m" << std::endl;
        i++;
    }

    for (int j = i; j < 10; j++) { std::cout << "║ (" << j << ") -" << std::endl; }
}

void GamePlayerConsole::displayPlayer(int p1, int p2) {
    displayTitle();
    int i = 1;

    for (auto name : _view->getPlayerName()) {
        std::cout << "║ (" << i << ") - ";
        if (i == p1) { std::cout << "\033[0;34m" << "*player 1* " << "\033[0m"; }
        if (i == p2) { std::cout << "\033[0;31m" << "*player 2* " << "\033[0m"; }
        std::cout << "\033[0;33m" << name << "\033[0m" << std::endl;
        i++;
    }

    for (int j = i; j < 10; j++) { std::cout << "║ (" << j << ") -" << std::endl; }
}

void GamePlayerConsole::displayOptions(int mode) {
    std::cout << "║\n╠════════════════════════════╗" << std::endl;
    
    if (mode == 2) {
        std::cout << "║ What do you want to do ?   ║" << std::endl;
        std::cout << "╠════════════════════════════╩══════════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ (1) Refresh" << std::endl;
        std::cout << "║ (2) Continue" << std::endl;
        std::cout << "║ (3) Leave" << std::endl;
    }
    else {
        std::cout << "║ Select the player number " << mode + 1 << " ║" << std::endl;
        std::cout << "╠════════════════════════════╩══════════════════════════════╪" << std::endl;
        std::cout << "║ (0)     Choose random" << std::endl;
        std::cout << "║ (1 - 9) Choose the player" << std::endl;
        std::cout << "║ (10)    Refresh" << std::endl;
        std::cout << "║ (11)    Leave" << std::endl;
    }
    std::cout << "╚═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GamePlayerConsole::display() {
    displayTitle();
    displayPlayer();
    displayOptions(0);
}

void GamePlayerConsole::display_error() {

}

void GamePlayerConsole::update() {

}

ReturnInput GamePlayerConsole::handle_input() {

}