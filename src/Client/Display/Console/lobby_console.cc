#include "lobby_console.hh"

LobbyConsole::LobbyConsole(const std::string &sessionId,
                           std::shared_ptr<GameClient> client)
    : _session_id(sessionId) {
  _view = std::make_shared<LobbyView>(client);
  _controller = std::make_shared<LobbyController>(client);
}

void LobbyConsole::display() {
  std::system("clear");
  displayFriends();
  displayOptions(_current_option);
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
  std::cout
      << "║\n║\n╠════════════════════════╗\n║What do you want to do ?║\n╠══"
         "══════════════════════╩══════════════════════════════════════════════"
         "══════════╪\n";
  switch (mode) {
  case 0:
    std::cout << "║ (1) Add Player to Lobby ⌨" << std::endl;
    std::cout << "║ (2) Start Game ⚑" << std::endl;
    std::cout << "║ (3) Refresh Player List ⌛" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════"
                 "══════════════╪\n";
    break;
  case 1:
    std::cout << "║ Enter a username to send a request!" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════"
                 "═════════════════════╪\n";
    break;
  default:
    std::cout << "║ Invalid option! Choose from the list below" << std::endl;
    std::cout << "║ (1) Add Player to Lobby ⌨" << std::endl;
    std::cout << "║ (2) Start Game ⚑" << std::endl;
    std::cout << "║ (3) Refresh Player List ⌛" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════"
                 "══════════════╪\n";
    break;
  }
}

ReturnInput LobbyConsole::handleInput() {
  int input;
  std::cin >> input;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    displayOptions(_current_option);
  }

  switch (input) {
  case 1: // Add Player to Lobby
    _current_option = 1;
    display();
    _current_option = 0;
    addPlayer();
    break;
  case 2: // Start Game
    // startGame();
    break;
  case 3: // Refresh Player List
    _current_option = 0;
    display();
    break;
  default:
    std::cout << "Invalid option. Please choose again.\n";
    display();
    break;
  }
  return {ReturnInput::LOBBY, _session_id};
}

void LobbyConsole::addPlayer() {
  std::string playerName;
  std::cout << "Type the name: ";
  std::cin >> playerName;

  std::string message = "sessionID: " + _session_id + "\n";
  std::cout << "le test : " << message << std::endl;
  _controller->sendIDGame(playerName, message);
}