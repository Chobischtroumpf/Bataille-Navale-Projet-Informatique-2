#pragma once

#include <string>

struct ReturnInput {
    enum Screen {
        MAIN_MENU,
        GAME,
        CHAT,
        LOBBY,
        LOGIN,
        REGISTER,
        GAME_CREATION
    };

    Screen screen;
    std::string arg;
};

/*
 * Interface du display (ce qui est affiché à l'écran)
 */
namespace screen {
    class Display {
    public:
        virtual void display() = 0;
        virtual void displayError() = 0;
        virtual void update() = 0;
        virtual ReturnInput handleInput() = 0;
    };
}

