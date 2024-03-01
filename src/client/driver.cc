#include "driver.hh"

// Controllers
#include "Controllers/game_controller.hh"
#include "Controllers/login_controller.hh"
#include "Controllers/register_controller.hh"
#include "Controllers/main_menu_controller.hh"
#include "Controllers/chat_controller.hh"

// Displays
#include "Display/Console/game_console.hh"
#include "Display/Console/login_console.hh"
#include "Display/Console/register_console.hh"
#include "Display/Console/main_menu_console.hh"
#include "Display/Console/chat_console.hh"

// Views
#include "display.hh"
#include "local_board.hh"
#include "Views/login_view.hh"
#include "Views/register_view.hh"
#include "Views/main_menu_view.hh"
#include "Views/chat_view.hh"

Driver::Driver(DisplayType display_type) : _display_type{display_type} {}

Driver::~Driver() {}

void Driver::launchApp() {
  run(ReturnInput::Screen::LOGIN);
}

void Driver::run(ReturnInput::Screen base_screen) {
    while (true) {
      _display->display();
      ReturnInput input = _display->handle_input();
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
    _view = std::make_shared<LocalBoard>();
    _controller = std::make_shared<GameController>(_view);
    _display = std::make_shared<GameConsole>(_controller, _view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLoginScreen() {
  if (_display_type == CONSOLE) {
    _view = std::make_shared<LoginView>();
    _controller = std::make_shared<LoginController>(_view);
    _display = std::make_shared<LoginConsole>(_controller, _view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayRegisterScreen() {
  if (_display_type == CONSOLE) {
    _view = std::make_shared<RegisterView>();
    _controller = std::make_shared<RegisterController>(_view);
    _display = std::make_shared<RegisterConsole>(_controller, _view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayMainMenuScreen() {
  if (_display_type == CONSOLE) {
    _view = std::make_shared<MainMenuView>();
    _controller = std::make_shared<MainMenuController>(_view);
    _display = std::make_shared<MainMenuConsole>(_controller, _view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayChatScreen(std::string username) {
  if (_display_type == CONSOLE) {
    _view = std::make_shared<ChatView>();
    _controller = std::make_shared<ChatController>(_view);
    _display = std::make_shared<ChatConsole>(_controller, _view);
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLobbyScreen() {}

void Driver::displayGameCreationScreen() {}
