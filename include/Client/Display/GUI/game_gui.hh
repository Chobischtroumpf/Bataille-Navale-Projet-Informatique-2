#pragma once

#include <memory>

#include "gui.hh"
#include "board_coordinates.hh"
#include "game_view.hh"

class GameGui : public GUI {
public:
    void display();
    void display_error();
    void update();
    void handle_input(BoardCoordinates user_input);
private:
    std::shared_ptr<GameView> _game_view;
};