#pragma once

#include <memory>

#include "main_menu_view.hh"
#include "console.hh"
#include "main_menu_controller.hh"
#include "display.hh"



class MainMenuConsole : public Console {
public:
    MainMenuConsole(std::shared_ptr<MainMenuView> view);
    void display() override;
    void displayError() override;
    void update() override;
    ReturnInput handleInput() override;
private:
    std::shared_ptr<MainMenuView> _view;
    std::shared_ptr<MainMenuController> _controller;
    int _friendlist_position = 4;
    void displayFriends();
    void displayNotifications();
    void displayOptions(int mode);
};
