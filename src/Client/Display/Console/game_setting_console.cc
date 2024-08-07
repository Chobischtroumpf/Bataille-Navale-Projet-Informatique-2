
#include "game_setting_console.hh"

GameSettingConsole::GameSettingConsole(std::shared_ptr<GameClient> gameClient)
    : gameClient(gameClient) {}

void GameSettingConsole::displayTitle() {
  system("clear");
  std::cout << "╔═══════════════════════════════════╗" << std::endl;
  std::cout << "║ Select the properties of the game ║" << std::endl;
  std::cout
      << "╠═══════════════════════════════════╩═══════════════════════╪\n║"
      << std::endl;
}

void GameSettingConsole::displayParameter() {
  displayTitle();
  std::cout << "║ Game name : " << "\033[0;33m" << _game_name << "\033[0m" << std::endl;
  std::cout << "╠═══════════════════════════════════════════════════════════╪"
            << std::endl;
  std::cout << "║ Game mode : " << "\033[0;33m";
  if (_commander_mode.has_value()) {
    if (_commander_mode.value()) {
      std::cout << "Commander"
                << "\033[0m" << std::endl;
    } else {
      std::cout << "Classic"
                << "\033[0m" << std::endl;
    }
  } else std::cout << "\033[0m" << std::endl;
  std::cout << "╠═══════════════════════════════════════════════════════════╪"
            << std::endl;
  std::cout << "║ Timer mode : " << "\033[0;33m";
  if (_timer_mode.has_value()) {
    if (_timer_mode.value()) {
      std::cout << "Classic"
                << "\033[0m" << std::endl;
    } else {
      std::cout << "Pendulum"
                << "\033[0m" << std::endl;
    }
  } else std::cout << "\033[0m" << std::endl;
  
  std::cout << "╠═══════════════════════════════════════════════════════════╪"
            << std::endl;
  std::cout << "║ Time per turn of the player : " << "\033[0;33m" << (_time_per_turn.has_value() ? (std::to_string(_time_per_turn.value()) + " seconds") : "") << "\033[0m" << std::endl;
  std::cout << "╠═══════════════════════════════════════════════════════════╪"
            << std::endl;
  std::cout << "║ Time per game of the player : " << "\033[0;33m" << (_time_per_game.has_value() ? (std::to_string(_time_per_game.value()) + " seconds") : "") << "\033[0m" << std::endl;
  std::cout << "╠═══════════════════════════════════════════════════════════╪"
            << std::endl;
  std::cout << "║ Spectator allowed : " << "\033[0;33m";
  if (_spectator_allowed.has_value()) {
    if (_spectator_allowed.value()) {
      std::cout << "yes"
                << "\033[0m" << std::endl;
    } else {
      std::cout << "no"
                << "\033[0m" << std::endl;
    }
  } else
    std::cout << "\033[0m" << std::endl;
  displayOptions(_option_counter);
}

void GameSettingConsole::displayOptions(int mode) {
    std::cout << "║\n╠════════════════════════════════════════╗" << std::endl;
    switch (mode) {
      case 0:
        std::cout << "║ Choose a game name                     ║" << std::endl;
        break;
      case 1:
        std::cout << "║ Choose a game mode                     ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║ (1) Classic" << std::endl;
        std::cout << "║ (2) Commander" << std::endl;
        break;
      case 2:
        std::cout << "║ Choose the timer mode                  ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║ (1) Classic" << std::endl;
        std::cout << "║ (2) Pendulum" << std::endl;
        break;
      case 3:
        std::cout << "║ Choose the time per turn               ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ (30 - 60) seconds" << std::endl;
        break;
      case 4:
        std::cout << "║ Choose the time per game of the player ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║" << std::endl;
        std::cout << "║ (60 - 1000) seconds" << std::endl;
        break;
      case 5:
        std::cout << "║ Choose if spectator are allowed        ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║ (1) Yes" << std::endl;
        std::cout << "║ (2) No" << std::endl;
        break;
      default:
        std::cout << "║ What do you want to do ?               ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╩══════════════════╪" << std::endl;
        std::cout << "║ (1) Continue " << std::endl;
        std::cout << "║ (2) Go back" << std::endl;
        break;
    }
    std::cout << "╚═══════════════════════════════════════════════════════════╪" << std::endl;
}

void GameSettingConsole::display() {
  displayTitle();
  displayParameter();
}

void GameSettingConsole::displayError() {}

void GameSettingConsole::update() {}

ReturnInput GameSettingConsole::handleInput() {
  std::string input;
  if (!_valid_input) {
    std::cout << "Invalid input, please try again" << std::endl;
  }
  std::cout << "Please enter your choice: ";
  std::cin >> input;
  if (_validators[_option_counter](input)) {
    switch (_option_counter) {
    case 0:
      _game_name = input;
      break;
    case 1:
      if (input == "1") {
        _commander_mode = false;
      } else {
        _commander_mode = true;
      }
      break;
    case 2:
      if (input == "1"){
        _timer_mode =true;
      }else{
        _timer_mode =false;
      }
      break;
    case 3:
      _time_per_turn = std::stoi(input);
      break;
    case 4:
      _time_per_game = std::stoi(input);
      break;
    case 5:
      if (input == "1") {
        _spectator_allowed = true;
      } else {
        _spectator_allowed = false;
      } 
      break;
    case 6:
      if (input == "1") {
        njson gameDetails = {{"name", _game_name},
                             {"gamemode", (_commander_mode.value() ? "Commander" : "Classic")},
                             {"classicTimer", (_timer_mode.value() ? "Classic" : "Commander")},
                             {"playerTimeLimit", _time_per_game.value()},
                             {"turnTimeLimit", _time_per_turn.value()},
                             {"maxPlayers", (_spectator_allowed.value() ? 8 : 2)}
                             };

        auto resultFuture = gameClient->CreateGame(gameDetails);
        auto gameID = resultFuture.get();
        // send the default faction
        if (_commander_mode.value()) {
          nlohmann::json req;
          req["moveType"] = "chooseFaction";
          req["faction"] = 0;
          gameClient->MakeMove(gameID, req);
        }
        return ReturnInput(ReturnInput::Screen::LOBBY, gameID);
      } else {
        return ReturnInput(ReturnInput::Screen::MAIN_MENU, "");
      }
      break;
    }
    _option_counter++;
    _valid_input = true;
  } else _valid_input = false;
  return ReturnInput(ReturnInput::Screen::GAME_CREATION, "");
}

bool GameSettingConsole::isCommanderMode() const {
  return _commander_mode.value();
}

bool GameSettingConsole::isClassicTimer() const {
  return _timer_mode.value();
}

bool GameSettingConsole::isSpectatorAllowed() const {
  return _spectator_allowed.value();
}

std::string GameSettingConsole::getGameName() const {
  return _game_name;
}

int GameSettingConsole::getTimePerTurn() const {
  return _time_per_turn.value();  
}

int GameSettingConsole::getTimePerGame() const {
  return _time_per_game.value();
}

