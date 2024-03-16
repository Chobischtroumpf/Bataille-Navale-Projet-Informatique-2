#pragma once

#include <memory>

#include "controller.hh"
#include "view.hh"
#include "game_client.hh"

// Controllers
#include "game_controller.hh"
#include "login_controller.hh"
#include "register_controller.hh"
#include "main_menu_controller.hh"
#include "chat_controller.hh"
#include "lobby_controller.hh"

// Displays
#include "display.hh"
#include "game_console.hh"
#include "login_console.hh"
#include "register_console.hh"
#include "main_menu_console.hh"
#include "chat_console.hh"
#include "game_setting_console.hh"
#include "lobby_console.hh"

// Views
#include "local_board.hh"
#include "login_view.hh"
#include "register_view.hh"
#include "main_menu_view.hh"
#include "chat_view.hh"
#include "lobby_view.hh"


enum DisplayType {
    CONSOLE,
    GUI
};

class Driver {
public:
    Driver(DisplayType display_type);
    ~Driver();

    // Launchers
    void launchApp();

    // Main loop
    void run(ReturnInput::Screen base_screen);

    // Getter
    std::shared_ptr<GameClient> getClient();

private:
    // Display a specific screen
    void displayGameScreen();
    void displayLoginScreen();
    void displayRegisterScreen();
    void displayMainMenuScreen();
    void displayChatScreen(std::string username);
    void displayLobbyScreen(std::string gameId);
    void displayGameCreationScreen();

    std::shared_ptr<Display> _display;

    DisplayType _display_type;
    ReturnInput::Screen _current_screen;
    std::shared_ptr<GameClient> _game_client;

};