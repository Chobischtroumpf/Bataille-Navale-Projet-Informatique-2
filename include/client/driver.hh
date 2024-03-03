#pragma once

#include <memory>
#include "controller.hh"
#include "display.hh"
#include "view.hh"

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

private:
    // Display a specific screen
    void displayGameScreen();
    void displayLoginScreen();
    void displayRegisterScreen();
    void displayMainMenuScreen();
    void displayChatScreen(std::string username);
    void displayLobbyScreen();
    void displayGameCreationScreen();

    std::shared_ptr<Display> _display;

    DisplayType _display_type;
    ReturnInput::Screen _current_screen;
};