#include "lobby_console.hh"

LobbyConsole::LobbyConsole(const std::string &session_id,
                           std::shared_ptr<GameClient> client,
                           bool admin)
    : _session_id(session_id) , _admin(admin) {
  _view = std::make_shared<LobbyView>(client);
  _controller = std::make_shared<LobbyController>(client);
}

void LobbyConsole::display() {
  std::system("clear");
  displayFriends();
  if (_commander_mode) {
    displayFactions();
  }
  if (_admin) {
    displayOptions(_current_option);
  } else {
    if (!_faction_chosen && _commander_mode) {
      displayOptions(2);
    } else {
      std::cout << "║ Please wait the game to start                            "
                   "            ║"
                << std::endl;

      std::cout << "╚";
      for (int i = 0; i < _width; i++)
        std::cout << "═";
      std::cout << "╝" << std::endl;
    }
  }
}

void LobbyConsole::displayFriends() {
  std::cout << "╔════════════╗\n║Player List ║\n╠════"
               "════════╩══════════════════════════════════════════════════════"
               "═══╗" << std::endl;
  for (auto user : _view->getUserInGame(_session_id)) {
    std::cout << "║ ";
    std::cout << "🔺 " << user << std::string(_width - user.size() - 4, ' ') << "║" << std::endl;
  }
  std::cout << "║" << std::string(_width, ' ') << "║" << std::endl;
  std::cout << "║ " << "\033[1m" << "Your invite link:" << std::string(_width - 18, ' ') << "\033[0m" << "║" << std::endl;
  std::cout << "║ " << _session_id << std::string(_width - _session_id.size() - 1, ' ') << "║" << std::endl;
  std::cout << "╠";
  for (int i = 0; i < _width; i++) std::cout << "═";
  std::cout << "╣" << std::endl;
}

void LobbyConsole::displayFactions() {
  for (int i = 1; i< 4; i++){
    std::cout << "║ " << (_selected_faction == i - 1 ? "\033[0;33m" : "\033[0m")
              << "(" << (_selected_faction == i - 1 ? ">" : std::to_string(i))
              << ") " << _factions[i - 1]
              << std::string(_width - (_factions[i - 1].size() + 3), ' ')
              << "\033[0m"
              << "║" << std::endl;
  }
  std::cout << "╠";
  for (int i = 0; i < _width; i++) std::cout << "═";
  std::cout << "╣" << std::endl;
}

void LobbyConsole::displayOptions(int mode) {
  switch (mode) {
  case 0:
    for (int i = 1; i < _options.size() + 1; i++) {
      std::cout << "║ (" << i << ") " << _options[i - 1]
                << std::string(_width - (_options[i - 1].size() + 3), ' ')
                << "║" << std::endl;
    }
    break;
  case 1:
    std::cout << "║ Enter a username to send a request                                   ║" << std::endl;
    std::cout << "║ Type :'./exit' to back to the lobby                                  ║" << std::endl;
    break;
  case 2:
    std::cout << "║ Choose a faction                                                     ║" << std::endl;
    break;
  }
  std::cout << "╚";
  for (int i = 0; i < _width; i++)
    std::cout << "═";
  std::cout << "╝" << std::endl;
}

ReturnInput LobbyConsole::handleStartGame() {
  std::clog << "handleStartGame" << std::endl;
  if (_view->getUserInGame(_session_id).size() < 2) {
    return {ReturnInput::Screen::LOBBY, _session_id};
  }
  _controller->launchGame(_session_id);
  return {ReturnInput::Screen::GAME, _session_id};
}

ReturnInput LobbyConsole::handleInvitePlayer() {
  std::clog << "handleInvitePlayer" << std::endl;
  std::string input;
  std::cin >> input;
  if (input == "./exit"){
    _current_option = 0;
    return {ReturnInput::Screen::LOBBY, _session_id};
  }
  else{
    addPlayer(input);
    _current_option = 0;
    return {ReturnInput::Screen::LOBBY, _session_id};
  }
}

ReturnInput LobbyConsole::GoToInvitePlayer() {
  std::clog << "GoToInvitePlayer" << std::endl;
  _current_option = 1;
  return {ReturnInput::Screen::LOBBY, _session_id};
}

ReturnInput LobbyConsole::refresh() {
  std::clog << "refresh" << std::endl;
  _current_option = 0;
  return {ReturnInput::Screen::LOBBY, _session_id};
}

ReturnInput LobbyConsole::backToMainMenu() {
  std::clog << "backToMainMenu" << std::endl;
  return {ReturnInput::Screen::MAIN_MENU, ""};
}

ReturnInput LobbyConsole::handleChoseFaction(int faction) {
  std::clog << "handleChoseFaction" << std::endl;
  _selected_faction = faction - 1;
  _current_option = 0;
  _faction_chosen = true;
  _controller->sendFaction(_session_id, _selected_faction);
  return {ReturnInput::Screen::LOBBY, _session_id};
}

ReturnInput LobbyConsole::handleInput() {
  std::clog << "handleInput" << std::endl;
  if (_admin) {
    int input;
    if (!_valid_input) {
      std::cout << "Invalid input, please try again" << std::endl;
      _valid_input = true;
    }
    std::cout << "Please enter your choice: ";
    if (_current_option != 1) {
      std::cin >> input;
    }

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return {ReturnInput::Screen::LOBBY, _session_id};
    }

    switch (_current_option) {
    case 0:
      switch (input) {
      case 1:
        return GoToInvitePlayer();
        break;
      case 2:
        if (_commander_mode) {
          _current_option = 2;
          return {ReturnInput::Screen::LOBBY, _session_id};
        } else return handleStartGame();
        break;
      case 3:
        if (_commander_mode) return handleStartGame();
        else return refresh();
        break;
      case 4:
        if (_commander_mode) return refresh();
        else return backToMainMenu();
        break;
      case 5:
        if (_commander_mode) return backToMainMenu();
        else {
          _valid_input = false;
          return {ReturnInput::Screen::LOBBY, _session_id};
        }
      }
      break;
    case 1:
      return handleInvitePlayer();
      break;
    case 2:
      return handleChoseFaction(input);
      break;
    default:
      _valid_input = false;
      return {ReturnInput::Screen::LOBBY, _session_id};
    }

  } else {
    if (!_faction_chosen && _commander_mode) {
      int input;
      std::cin >> input;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return {ReturnInput::Screen::LOBBY, _session_id};
      }
      return handleChoseFaction(input);
    } else {
      _view->waitGameStart(_session_id);
      return {ReturnInput::Screen::GAME, _session_id};
    }
  }
}

void LobbyConsole::addPlayer(const std::string& playerName) {
    std::string message = "sessionID: " + _session_id;
    _controller->sendIDGame(playerName, message);
}

void LobbyConsole::loadParameters(std::shared_ptr<GameSettingConsole> gameSettingConsole) {
  _max_players = gameSettingConsole->isSpectatorAllowed() ? 8 : 2;
  _game_name = gameSettingConsole->getGameName();
  _commander_mode = gameSettingConsole->isCommanderMode();
  _options = _commander_mode ? _options_commander : _options_classic;
}

void LobbyConsole::loadParameters(const std::string& session_id) {
  nlohmann::json game = _view->getGameState(session_id);
  _commander_mode = game.at("gameState").at("gamemode") == "Commandant";
  _game_name = game.at("sessionName").get<std::string>();
}

bool LobbyConsole::isCommanderMode() const {
  return _commander_mode;
}

int LobbyConsole::getFaction() const {
  return _selected_faction;
}