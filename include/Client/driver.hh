#pragma once

#include <memory>

// #include "controller.hh"
#include "game_client.hh"
// #include "view.hh"

// Controllers
// #include "chat_controller.hh"
// #include "game_controller.hh"
// #include "lobby_controller.hh"
// #include "login_controller.hh"
// #include "main_menu_controller.hh"
// #include "register_controller.hh"

// Displays
#include "display.hh"

// Views
// #include "chat_view.hh"
// #include "lobby_view.hh"
// #include "local_board_commander.hh"
// #include "login_view.hh"
// #include "main_menu_view.hh"
// #include "register_view.hh"

enum DisplayType { CONSOLE, GUI };

class Driver {
private:
  // Display a specific screen
  void displayGameScreen(std::string gameId);
  void displayLoginScreen();
  void displayRegisterScreen();
  void displayMainMenuScreen();
  void displayChatScreen(std::string username);
  void displayLobbyScreen(std::string gameId, bool admin);
  void displayGameCreationScreen();

  std::shared_ptr<Display> _display;

  DisplayType _display_type;
  ReturnInput::Screen _current_screen;
  std::shared_ptr<GameClient> _game_client;

public:
  Driver(DisplayType display_type, std::string server_address = "http://localhost:8080");
  ~Driver();

  // Launchers
  void launchApp();

  // Main loop
  void run(ReturnInput::Screen base_screen);

  // Getter
  std::shared_ptr<GameClient> getClient();
};