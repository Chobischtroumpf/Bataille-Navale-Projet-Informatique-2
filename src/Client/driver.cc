#include "driver.hh"

Driver::Driver(DisplayType display_type) : _display_type{display_type}, _game_client{std::make_shared<GameClient>("http://localhost:8080")} {}

Driver::~Driver() {}

void Driver::launchApp() {
  //std::string user = "slectedUser";
  displayLoginScreen();
  run(ReturnInput::Screen::LOGIN);
}

void Driver::run(ReturnInput::Screen base_screen) {
    while (true) {
      _display->display();
      ReturnInput input = _display->handleInput();
      if (_current_screen != input.screen)
      switch (input.screen) {
        case ReturnInput::Screen::MAIN_MENU:
          displayMainMenuScreen(input.arg);
          break;
        case ReturnInput::Screen::GAME:
          displayGameScreen();
          break;
        case ReturnInput::Screen::CHAT:
          displayChatScreen(input.arg);
          break;
        case ReturnInput::Screen::LOBBY:
          displayLobbyScreen(input.arg);
          break;
        case ReturnInput::Screen::LOGIN:
          displayLoginScreen();
          break;
        case ReturnInput::Screen::REGISTER:
          displayRegisterScreen();
          break;
        case ReturnInput::Screen::GAME_CREATION:
          displayGameCreationScreen();
          break;
      }
    }
}

std::shared_ptr<GameClient> Driver::getClient() {
    return _game_client;
}

void Driver::displayGameScreen(std::string gameId) {
  if (_display_type == CONSOLE) {
    std::shared_ptr<LocalBoard> board = std::make_shared<LocalBoard>();
    std::shared_ptr<GameController> game_controller = std::make_shared<GameController>(board);
    _display = std::make_shared<GameConsole>(std::cout, std::cin, board, game_controller, getClient());
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLoginScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<LoginConsole>(getClient());
    _current_screen = ReturnInput::Screen::LOGIN;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayRegisterScreen() {
    // Ce display sert-il ?
  if (_display_type == CONSOLE) {
    _display = std::make_shared<RegisterConsole>();
    _current_screen = ReturnInput::Screen::REGISTER;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayMainMenuScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<MainMenuConsole>(getClient());
    _current_screen = ReturnInput::Screen::MAIN_MENU;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayChatScreen(std::string username) {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<ChatConsole>(username, getClient());
    _current_screen = ReturnInput::Screen::CHAT;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLobbyScreen(std::string gameId) {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<LobbyConsole>(gameId, getClient());
    _current_screen = ReturnInput::Screen::LOBBY;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayGameCreationScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<GameSettingConsole>(getClient());
    _current_screen = ReturnInput::Screen::GAME_CREATION;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}