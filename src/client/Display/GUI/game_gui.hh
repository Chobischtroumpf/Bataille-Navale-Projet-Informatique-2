#pragma once

#include "../gui.hh"
#include "../../../common/board_coordinates.hh"
#include "../../Views/game_view.hh"
#include <memory>

class GameGui : public GUI {
public:
    void display();
    void display_error();
    void update();
    void handle_input(BoardCoordinates user_input);
private:
    std::shared_ptr<GameView> _game_view;
};