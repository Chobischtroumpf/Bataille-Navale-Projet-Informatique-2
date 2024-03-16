#pragma once

#include <memory>

#include "main_menu_view.hh"
#include "console.hh"
#include "main_menu_controller.hh"
#include "display.hh"
#include "game_client.hh"



class MainMenuConsole : public Console {
public:
    explicit MainMenuConsole(std::shared_ptr<GameClient> client);
    void display() override;
    void displayError() override;
    void update() override;
    ReturnInput handleInput() override;
private:
    std::shared_ptr<MainMenuView> _view;
    std::shared_ptr<MainMenuController> _controller;
    int _friendlist_position = 4;
    short _current_option = 0;
    void displayFriends();
    void displayNotifications();
    void displayOptions(int mode);
    void resetFriendListPosition();
};
