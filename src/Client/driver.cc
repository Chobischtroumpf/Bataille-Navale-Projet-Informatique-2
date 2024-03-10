#include "driver.hh"

Driver::Driver(DisplayType display_type) : _display_type{display_type} {}

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
          displayMainMenuScreen();
          break;
        case ReturnInput::Screen::GAME:
          displayGameScreen();
          break;
        case ReturnInput::Screen::CHAT:
          displayChatScreen(input.arg);
          break;
        case ReturnInput::Screen::LOBBY:
          displayLobbyScreen();
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

void Driver::displayGameScreen() {
  if (_display_type == CONSOLE) {
    std::shared_ptr<LocalBoard> board = std::make_shared<LocalBoard>();
    std::shared_ptr<GameController> game_controller = std::make_shared<GameController>(board);
    _display = std::make_shared<GameConsole>(std::cout, std::cin, board, game_controller);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLoginScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<LoginConsole>("http://localhost:8080");
    _current_screen = ReturnInput::Screen::LOGIN;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayRegisterScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<RegisterConsole>();
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayMainMenuScreen() {
  if (_display_type == CONSOLE) {
    std::shared_ptr<MainMenuView> _view = std::make_shared<MainMenuView>();
    _display = std::make_shared<MainMenuConsole>(_view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayChatScreen(std::string username) {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<ChatConsole>("me",username);
    _current_screen = ReturnInput::Screen::CHAT;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLobbyScreen() {}

void Driver::displayGameCreationScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<GamePlayerConsole>();

  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}
