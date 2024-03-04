#include "../../../../include/client/Display/Console/game_setting_console.hh"

GameSettingConsole::GameSettingConsole(std::shared_ptr<GameSettingView> view): _view(view) {}

void GameSettingConsole::displayTitle() {
    std::cout << "╔═══════════════════════════════════╗" << std::endl;
    std::cout << "║ Select the properties of the game ║" << std::endl;
    std::cout << "╠═══════════════════════════════════╩═══════════════════════╪\n║\n║" << std::endl;
}

void GameSettingConsole::displayParameter() {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Number of boats : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::displayParameter(std::string game_name) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Number of boats : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::displayParameter(std::string game_name, int number_boat) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Number of boats : " << number_boat << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::displayParameter(std::string game_name, int number_boat, bool spectator_allowed) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Number of boats : " << number_boat << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : ";
    if (spectator_allowed) { std::cout << "yes" << std::endl; }
    else { std::cout << "no" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::displayOptions(int mode) {
    std::cout << "\n║\n║\n╠═════════════════════════════════╗" << std::endl;
    switch (mode) {
      case 0:
        std::cout << "║ Choose a game name              ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║" << std::endl;
        break;
      case 1:
        std::cout << "║ Choose a number of boats        ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ 1 - 15" << std::endl;
        break;
      case 2:
        std::cout << "║ Choose if spectator are allowed ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║ (1) Yes" << std::endl;
        std::cout << "║ (2) No" << std::endl;
        break;
      default:
        std::cout << "║ What do you want to do ?        ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║ (1) Continue " << std::endl;
        std::cout << "║ (2) Go back" << std::endl;
        break;
    }
    std::cout << "╚═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::display() {
    displayTitle();
    displayParameter();
    displayOptions(0);
}

void GameSettingConsole::display_error() {

}

void GameSettingConsole::update() {

}

ReturnInput GameSettingConsole::handle_input() {
    
}