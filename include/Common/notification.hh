#pragma once

#include <string>
/*
 * Représentation d'une notification
 */

enum Screen {
    MAIN_MENU,
    GAME,
    CHAT,
    LOBBY,
    LOGIN,
    REGISTER,
    GAME_CREATION
};

class Notification {
private:
    std::string _text;
    Screen _screen;

public:
    Notification(Screen screen, std::string& message);

    const std::string& getText() const;
    const Screen getScreen() const;
};
