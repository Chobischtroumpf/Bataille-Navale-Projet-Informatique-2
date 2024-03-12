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
    void displayError() override;
    void update() override;
    ReturnInput handleInput() override;
private:
    std::shared_ptr<GameSettingView> _view;
    std::shared_ptr<GameSettingController> _controller;
    int _friendlist_position = 4;
    void displayTitle();
    void displayParameter();
    void displayParameter(std::string game_name);
    void displayParameter(std::string game_name, bool game_mode_classic);
    void displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn);
    void displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn, int time_per_game);
    void displayParameter(std::string game_name, bool game_mode_classic, int time_per_turn, int time_per_game, bool spectator_allowed);
    ReturnInput displayOptions(int mode);
};