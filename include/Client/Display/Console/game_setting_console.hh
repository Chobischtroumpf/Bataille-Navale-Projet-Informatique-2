#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "../../Views/game_setting_view.hh"
#include "../console.hh"
#include "../../Controllers/game_setting_controller.hh"


class GameSettingConsole : public Console {
public:
    GameSettingConsole(std::shared_ptr<GameSettingView> view);
    void display() override;
    void display_error() override;
    void update() override;
    void handle_input() override;
private:
    std::shared_ptr<GameSettingView> _view;
    std::shared_ptr<GameSettingController> _controller;
    int _friendlist_position = 4;
    void displayTitle();
    void displayParameter();
    void displayParameter(std::string game_name);
    void displayParameter(std::string game_name, int number_boat);
    void displayParameter(std::string game_name, int number_boat, bool spectator_allowed);
    void displayOptions(int mode);
};