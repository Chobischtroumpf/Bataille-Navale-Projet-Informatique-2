#include "lobby_console.hh"

LobbyConsole::LobbyConsole(const std::string &sessionId,
                           std::shared_ptr<GameClient> client,
                           bool admin)
    : _session_id(sessionId) , _admin(admin) {
  _view = std::make_shared<LobbyView>(client);
  _controller = std::make_shared<LobbyController>(client);
}

void LobbyConsole::display() {
  std::system("clear");
  displayFriends();
  if (_admin) {
    displayOptions(_current_option);
  }
}

void LobbyConsole::displayFriends() {
  std::cout << "╔════════════╗\n║Player List ║\n╠════"
               "════════╩══════════════════════════════════════════════════════"
               "══════════════╪\n║\n║ ";
  for (auto user : _view->getUserInGame(_session_id)) {
    std::cout << "║ ";
    std::cout << user << std::endl;
  }
  std::cout << "╠══════════════════════════════════════════════════════════════"
               "══════╪\n";
}

void LobbyConsole::displayOptions(int mode) {
    switch (mode) {
        case 0:
            std::cout << "║ (1) Add Player to Lobby ⌨" << std::endl;
            std::cout << "║ (2) Start Game ⚑" << std::endl;
            std::cout << "║ (3) Refresh Player List ⌛" << std::endl;
            std::cout << "║ (4) Back to the Main Menu ☝" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        case 1:
            std::cout << "║ Enter a username to send a request" << std::endl;
            std::cout << "║ Type :'./exit' to back to the lobby"<< std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
        default:
            std::cout << "║ Invalid option! Choose from the list below" << std::endl;
            std::cout << "║ (1) Add Player to Lobby ⌨" << std::endl;
            std::cout << "║ (2) Start Game ⚑" << std::endl;
            std::cout << "║ (3) Refresh Player List ⌛" << std::endl;
            std::cout << "║ (4) Back to the Main Menu ☝" << std::endl;
            std::cout << "╚═════════════════════════════════════════════════════════════════════════════════╪\n";
            break;
    }
}

void LobbyConsole::wait() {
  bool game_started = false;
  while (!game_started) {
  }
  std::cout << "Game is starting!" << std::endl;
}

ReturnInput LobbyConsole::handleInput() {
  if (_admin) {
  int input;
  std::cout << "Please enter your choice: ";
  std::cin >> input;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    displayOptions(_current_option);
  }

    switch (input) {
        case 1: {// Add Player to Lobby
            _current_option = 1;
            display();
            _current_option = 0;
            std::string playerName;
            std::cout << "Please enter your text: ";
            std::cin >> playerName;
            if (playerName =="./exit")
                break;
            else
                addPlayer(playerName);
            break;
        }
        case 2: // Start Game
            if (_view->getUserInGame(_session_id).size() < 2) {
                break;
            }
            return {ReturnInput::Screen::GAME, _session_id};
            break;
        case 3: // Refresh Player List
            _current_option = 0;
            display();
            break;
        case 4: // back to the Main Menu
            return {ReturnInput::MAIN_MENU,""};
        default:
            std::cout << "Invalid option. Please choose again.\n";
            display();
            break;
    }
  } else {
    _view->waitGameStart(_session_id);
  }   
    return {ReturnInput::LOBBY, sessionId};
}

void LobbyConsole::addPlayer(const std::string& playerName) {
    std::string message = "sessionID: " + sessionId;
    _controller->sendIDGame(playerName, message);
}