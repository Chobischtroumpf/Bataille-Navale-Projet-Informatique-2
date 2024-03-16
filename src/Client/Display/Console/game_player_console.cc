#include "game_player_console.hh"

GamePlayerConsole::GamePlayerConsole(std::shared_ptr<GamePlayerView> view)
    : _view(view) {}

void GamePlayerConsole::displayTitle() {
  std::cout << "╔═══════════════════════════════╗" << std::endl;
  std::cout << "║ Select the player of the game ║" << std::endl;
  std::cout
      << "╠═══════════════════════════════╩═══════════════════════════╪\n║"
      << std::endl;
}

void GamePlayerConsole::displayPlayer() {
  displayTitle();
  int i = 1;

  for (auto name : _view->getPlayerName()) {
    std::cout << "║ (" << i << ") - "
              << "\033[0;33m" << name << "\033[0m" << std::endl;
    i++;
  }

  for (int j = i; j < 9; j++) {
    std::cout << "║ (" << j << ") -" << std::endl;
  }
  displayOptions(0);
}

void GamePlayerConsole::displayPlayer(int p1) {
  displayTitle();
  int i = 1;

  for (auto name : _view->getPlayerName()) {
    std::cout << "║ (" << i << ") - ";
    if (i == p1) {
      std::cout << "\033[0;34m"
                << "*player 1* "
                << "\033[0m";
    }
    std::cout << "\033[0;33m" << name << "\033[0m" << std::endl;
    i++;
  }

  for (int j = i; j < 9; j++) {
    std::cout << "║ (" << j << ") -" << std::endl;
  }
  displayOptions(0);
}

void GamePlayerConsole::displayPlayer(int p1, int p2) {
  displayTitle();
  int i = 1;

  for (auto name : _view->getPlayerName()) {
    std::cout << "║ (" << i << ") - ";
    if (i == p1) {
      std::cout << "\033[0;34m"
                << "*player 1* "
                << "\033[0m";
    }
    if (i == p2) {
      std::cout << "\033[0;31m"
                << "*player 2* "
                << "\033[0m";
    }
    std::cout << "\033[0;33m" << name << "\033[0m" << std::endl;
    i++;
  }

  for (int j = i; j < 9; j++) {
    std::cout << "║ (" << j << ") -" << std::endl;
  }
  displayOptions(1);
}

void GamePlayerConsole::displayOptions(int mode) {
  std::cout << "║\n╠════════════════════════════╗" << std::endl;

  if (mode == 0) {
    std::cout << "║ Select the player number " << mode + 1 << " ║" << std::endl;
    std::cout << "╠════════════════════════════╩══════════════════════════════╪"
              << std::endl;
    std::cout << "║ (0)     Choose random" << std::endl;
    std::cout << "║ (1 - 8) Choose the player" << std::endl;
    std::cout << "║ (10)    Refresh" << std::endl;
    std::cout << "║ (11)    Leave" << std::endl;
  } else {
    std::cout << "║ What do you want to do ?   ║" << std::endl;
    std::cout << "╠════════════════════════════╩══════════════════════════════╪"
              << std::endl;
    std::cout << "║" << std::endl;
    std::cout << "║ (1) Rechoose" << std::endl;
    std::cout << "║ (2) Refresh" << std::endl;
    std::cout << "║ (3) Continue" << std::endl;
    std::cout << "║ (4) Leave" << std::endl;
  }
  std::cout << "╚═══════════════════════════════════════════════════════════╪"
            << std::endl;
}

void GamePlayerConsole::display() {
  displayTitle();
  displayPlayer();
  displayOptions(0);
}

void GamePlayerConsole::displayError() {}

void GamePlayerConsole::update() {}

ReturnInput GamePlayerConsole::handleInput() {
  std::srand(std::time(nullptr));
  bool next = false;
  int p1 = 0;
  int p2 = 0;
  std::string answer;

  while (!next) {
    bool invalid_input = true;
    bool input_error = false;

    while (invalid_input) {
      displayPlayer();

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
            p1 = std::rand() % _view->getPlayerName().size();
            p1++;
            invalid_input = false;
            break;
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
            p1 = value;
            if (p1 < _view->getPlayerName().size()) {
              invalid_input = false;
            }
            break;
          case 9:
          case 10:
            input_error = false;
            break;
          case 11:
            return {};
          default:
            break;
          }
        } catch (const std::exception &e) {
        }
      }
    }

    input_error = false;

    while (invalid_input) {
      displayPlayer(p1);

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
            if (_view->getPlayerName().size() > 1) {
              p2 = std::rand() % _view->getPlayerName().size();
              p2++;
              while (p2 == p1) {
                p2 = std::rand() % _view->getPlayerName().size();
                p2++;
              }
              invalid_input = false;
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
            p2 = value;
            if ((p2 < _view->getPlayerName().size()) and (p2 != p1)) {
              invalid_input = false;
            }
            break;
          case 10:
            input_error = false;
            break;
          case 11:
            return {};
          default:
            break;
          }
        } catch (const std::exception &e) {
        }
      }
    }

    input_error = false;

    while (invalid_input) {
      displayPlayer(p1, p2);

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
          case 1:
            invalid_input = false;
            break;
          case 2:
            input_error = false;
            break;
          case 3:
            invalid_input = false;
            next = true;
            break;
          case 4:
            return {};
          default:
            break;
          }
        } catch (const std::exception &e) {
        }
      }
    }
  }
}