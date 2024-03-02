#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "../../Views/main_menu_view.hh"
#include "../console.hh"
#include "../../Controllers/main_menu_controller.hh"



class MainMenuConsole : public Console {
public:
    MainMenuConsole(std::shared_ptr<MainMenuView> view);
    void display() override;
    void display_error() override;
    void update() override;
    ReturnInput handle_input() override;
private:
    std::shared_ptr<MainMenuView> _view;
    std::shared_ptr<MainMenuController> _controller;
    int _friendlist_position = 4;
    void displayFriends();
    void displayNotifications();
    void displayOptions(int mode);
};
