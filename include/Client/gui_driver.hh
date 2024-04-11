#pragma once

#include <memory>
#include "gui_LoginWindow.hh"
#include "game_client.hh"

class DriverGui {
public:
    DriverGui();
    void run();

private:
    std::shared_ptr<GameClient> _game_client;
    std::unique_ptr<LoginWindow> _loginWindow;
};
