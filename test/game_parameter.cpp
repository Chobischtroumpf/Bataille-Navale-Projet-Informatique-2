#include <iostream>
#include <cctype>
#include <random>
#include <string>
#include <ctime>
#include <vector>
#include "clear.hpp"


void draw1(int num, std::string game_name, bool game_mode_classic, int number_boat, bool spectator_allowed) {
    clear_screen();
    std::cout << "╔═══════════════════════════════════╗" << std::endl;
    std::cout << "║ Select the properties of the game ║" << std::endl;
    std::cout << "╠═══════════════════════════════════╩═══════════════════════╪\n║\n║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game name : ";
    if (num > 0) { std::cout << game_name; }
    std::cout << "\n╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Game mode : ";
    if (num > 1) {
        if (game_mode_classic) { std::cout << "classic"; }
        else { std::cout << "commander"; }
    }
    std::cout << "\n╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Number of boats : ";
    if (num > 2) { std::cout << number_boat; }
    std::cout << "\n╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║ Spectator allowed : ";
    if (num > 3) {
        if (spectator_allowed) { std::cout << "yes"; }
        else { std::cout << "no"; }
    }
    std::cout << "\n╠═══════════════════════════════════════════════════════════╪" << std::endl;
    std::cout << "║\n║\n╠═════════════════════════════════╗" << std::endl;
    switch (num) {
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
        std::cout << "║ Choose a number of boats        ║" << std::endl;
        std::cout << "╠═════════════════════════════════╩═════════════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ 1 - 15" << std::endl;
        break;
      case 3:
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

void draw2(int num, std::vector<std::string> names, int p1, int p2) {
    clear_screen();
    std::cout << "╔═══════════════════════════════╗" << std::endl;
    std::cout << "║ Select the player of the game ║" << std::endl;
    std::cout << "╠═══════════════════════════════╩═══════════════════════════╪\n║" << std::endl;
        
    int i = 1;
    for (auto name : names) {
        std::cout << "║ (" << i << ") - ";
        if ((num > 0) and (i == p1)) { std::cout << "*player 1* "; }
        else if ((num > 1) and (i == p2)) { std::cout << "*player 2* "; }
        std::cout << name << std::endl;
        i++;
    }
    
    for (int j = i; j < 10; j++) { std::cout << "║ (" << j << ") -" << std::endl; }
    
    std::cout << "║\n╠════════════════════════════╗" << std::endl;
    
    if (num == 0) { std::cout << "║ Select the player number " << num + 1 << " ║" << std::endl; }
    else { std::cout << "║ What do you want to do ?   ║" << std::endl; }
    
    std::cout << "╠════════════════════════════╩══════════════════════════════╪" << std::endl;
    if (num == 2) {
        std::cout << "║" << std::endl;
        std::cout << "║ (1) Refresh" << std::endl;
        std::cout << "║ (2) Continue" << std::endl;
        std::cout << "║ (3) Leave" << std::endl;
    }
    else {
        std::cout << "║ (0)     Choose random" << std::endl;
        std::cout << "║ (1 - 9) Choose the player" << std::endl;
        std::cout << "║ (10)    Refresh" << std::endl;
        std::cout << "║ (11)    Leave" << std::endl;
    }
    std::cout << "╚═══════════════════════════════════════════════════════════╪" << std::endl;
}

bool SelectParameter() {
    bool invalid_input = true;
    bool input_error = false;
    std::string game_name;
    std::string answer;
    bool game_mode_classic;
    int number_boat;
    bool spectator_allowed;
    
    while (invalid_input) {
        draw1(0, game_name, game_mode_classic, number_boat, spectator_allowed);
        
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
        draw1(1, game_name, game_mode_classic, number_boat, spectator_allowed);
        
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
        draw1(2, game_name, game_mode_classic, number_boat, spectator_allowed);
        
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
                number_boat = stoi(answer);
                
                if ((number_boat > 0) and (number_boat < 16)) {
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
        draw1(3, game_name, game_mode_classic, number_boat, spectator_allowed);
        
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
        draw1(4, game_name, game_mode_classic, number_boat, spectator_allowed);
        
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
                    return false;
                }
            }
            catch (const std::exception& e) {}
        }
	}
    return true;
}

bool SelectPlayer(std::vector<std::string> names) {
    std::srand(std::time(nullptr));
    
    int num = 0;
    int p1 = 2;
    int p2 = 3;
    std::string answer;
    bool input_error = false;
    bool invalid_input = true;
    
    while (num < 2) {
        draw2(num, names, p1, p2);
        
        if (input_error) {
            std::cout << "Invalid Input !";
        }
        std::cout << std::endl;
		
        input_error = true;
        std::getline(std::cin, answer);
        if (!answer.empty()) {
            try {
                int value = stoi(answer);
                switch (value) {
                  case 0:
                    if (num == 0) {
                        p1 = std::rand() % names.size() + 1;
                        num++;
                        input_error = false;
                    }
                    else if (names.size() > 1) {
                        p2 = std::rand() % names.size() + 1;
                        while (p2 == p1) {
                            p2 = std::rand() % names.size() + 1;
                        }
                        num++;
                        input_error = false;
                    }
                    break;
                  case 1:
                  case 2:
                  case 3:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                  case 8:
                  case 9:
                    if (num == 0) {
                        if (value < names.size()) {
                            p1 = value;
                            num++;
                            input_error = false;
                        }
                    }
                    else if ((value < names.size()) and (p2 == p1)) {
                        p2 = value;
                        num++;
                        input_error = false;
                    }
                    break;
                  case 10:
                    input_error = false;
                    break;
                  case 11:
                    return false;
                default:
                    break;
                }
            }
            catch (const std::exception& e) {}
        }
	}
    
    while (invalid_input) {
        draw2(2, names, p1, p2);
        
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
                    input_error = false;
                    break;
                  case 2:
                    invalid_input = false;
                    break;
                  case 3:
                    return false;
                  default:
                    break;
                }
            }
            catch (const std::exception& e) {}
        }
	}
    
    return true;
}

int main() {
    std::vector<std::string> names = {"name1", "name2", "name3", "name4", "name5"};
    bool next = SelectParameter();
    
    if (next) {
        next = SelectPlayer(names);
    }
    else {
        clear_screen();
    }
    
    if (!next) {
        clear_screen();
    }
    
    return 0;
}

// ╝ ║ ╚ ╗ ═ ╔ ╣ ╩ ╠ ╦ ╪ ╬