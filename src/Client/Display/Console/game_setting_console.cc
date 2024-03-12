#include "../../../../include/client/Display/Console/game_setting_console.hh"

GameSettingConsole::GameSettingConsole(std::shared_ptr<GameSettingView> view): _view(view) {}

void GameSettingConsole::displayTitle() {
    std::cout << "╔═══════════════════════════════════╗" << std::endl;
    std::cout << "║ Select the properties of the game ║" << std::endl;
    std::cout << "╠═══════════════════════════════════╩═══════════════════════╪\n║" << std::endl;
}

void GameSettingConsole::displayParameter() {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(0);
}

void GameSettingConsole::displayParameter(std::string game_name) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(1);
}

void GameSettingConsole::displayParameter(std::string game_name, bool game_mode_classic) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : ";
    if (game_mode_classic) { std::cout << "classic" << std::endl; }
    else { std::cout << "commander" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(2);
}

void GameSettingConsole::displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : ";
    if (game_mode_classic) { std::cout << "classic" << std::endl; }
    else { std::cout << "commander" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << time_per_turn << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(3);
}

void GameSettingConsole::displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn, int time_per_game) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : ";
    if (game_mode_classic) { std::cout << "classic" << std::endl; }
    else { std::cout << "commander" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << time_per_turn << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << time_per_game << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : " << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(4);
}

void GameSettingConsole::displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn, int time_per_game, bool spectator_allowed) {
    displayTitle();
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : " << game_name << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : ";
    if (game_mode_classic) { std::cout << "classic" << std::endl; }
    else { std::cout << "commander" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per turn : " << time_per_turn << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Time per game : " << time_per_game << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : ";
    if (spectator_allowed) { std::cout << "yes" << std::endl; }
    else { std::cout << "no" << std::endl; }
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    displayOptions(5);
}

void GameSettingConsole::displayOptions(int mode) {
    std::cout << "\n║\n╠═════════════════════════════════╗" << std::endl;
    switch (mode) {
      case 0:
        std::cout << "║ Choose a game name              ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║" << std::endl;
        break;
      case 1:
        std::cout << "║ Choose a game mode              ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║ (1) Classic" << std::endl;
        std::cout << "║ (2) Commander" << std::endl;
        break;
      case 2:
        std::cout << "║ Choose the time per turn        ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ (5 - 100) seconds" << std::endl;
        break;
      case 3:
        std::cout << "║ Choose the time per game        ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ (30 - 1000) seconds" << std::endl;
        break;
      case 4:
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

void GameSettingConsole::displayError() {

}

void GameSettingConsole::update() {

}

ReturnInput GameSettingConsole::handleInput() {
    bool invalid_input = true;
    bool input_error = false;
    std::string game_name;
    std::string answer;
    bool game_mode_classic;
    int time_per_turn;
    int time_per_game;
    bool spectator_allowed;
    
    while (invalid_input) {
        displayParameter();
        
        if (input_error) {
            std::cout << "Invalid Input !" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        
        input_error = true;
        std::getline(std::cin, game_name);
        if (!game_name.empty()) {
            invalid_input = false;
            for (char c : game_name) {
                if (!isalnum(c)) {
                    invalid_input = true;
                }
            }
        }
    }

    invalid_input = true;
    input_error = false;
    
    while (invalid_input) {
        displayParameter(game_name);
        
        if (input_error) {
            std::cout << "Invalid Input !" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                switch (stoi(answer)) {
                  case 1:
                    game_mode_classic = true;
                    invalid_input = false;
                    break;
                  case 2:
                    game_mode_classic = false;
                    invalid_input = false;
                    break;
                default:
                    break;
                }
            }
            catch (const std::exception& e) {}
        }
	}

    invalid_input = true;
    input_error = false;
    
    while (invalid_input) {
        displayParameter(game_name, game_mode_classic);
        
        if (input_error) {
            std::cout << "Invalid Input !" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                time_per_turn = stoi(answer);
                
                if ((time_per_turn > 4) and (time_per_turn < 101)) {
                    invalid_input = false;
                }
            }
            catch (const std::exception& e) {
            }
        }
	}

    invalid_input = true;
    input_error = false;
    
    while (invalid_input) {
        displayParameter(game_name, game_mode_classic, time_per_turn);
        
        if (input_error) {
            std::cout << "Invalid Input !" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                time_per_game = stoi(answer);
                
                if ((time_per_game > 29) and (time_per_game < 1001)) {
                    invalid_input = false;
                }
            }
            catch (const std::exception& e) {
            }
        }
	}

    invalid_input = true;
    input_error = false;
    
    while (invalid_input) {
        displayParameter(game_name, game_mode_classic, time_per_turn, time_per_game);
        
        if (input_error) {
            std::cout << "Invalid Input !";
        }
        std::cout << std::endl;
		
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                switch (stoi(answer)) {
                  case 1:
                    spectator_allowed = true;
                    invalid_input = false;
                    break;
                  case 2:
                    spectator_allowed = false;
                    invalid_input = false;
                    break;
                default:
                    break;
                }
            }
            catch (const std::exception& e) {}
        }
	}

    invalid_input = true;
    input_error = false;
    
    while (invalid_input) {
        displayParameter(game_name, game_mode_classic, time_per_turn, time_per_game, spectator_allowed);
        
        if (input_error) {
            std::cout << "Invalid Input !" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                if (stoi(answer) == 1) {
                    invalid_input = false;
                }
                else {
                    return;
                }
            }
            catch (const std::exception& e) {}
        }
	}
}