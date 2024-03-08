#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <random>
#include <ctime>
#include "../../Views/game_player_view.hh"
#include "../console.hh"
#include "../../Controllers/game_player_controller.hh"


class GamePlayerConsole : public Console {
public:
    GamePlayerConsole(std::shared_ptr<GamePlayerView> view);
    void display() override;
    void display_error() override;
    void update() override;
    ReturnInput handle_input() override;
private:
    std::shared_ptr<GamePlayerView> _view;
    std::shared_ptr<GamePlayerController> _controller;
    int _friendlist_position = 4;
    void displayTitle();
    void displayPlayer();
    void displayPlayer(int p1);
    void displayPlayer(int p1, int p2);
    void displayOptions(int mode);
};