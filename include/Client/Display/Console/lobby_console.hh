#pragma once

#include <memory>

#include "lobby_view.hh"
#include "console.hh"
#include "lobby_controller.hh"
#include "display.hh"
#include "game_client.hh"

class LobbyConsole : public Console {
private:
    std::vector<std::string> playersInLobby;
    std::string sessionId;
    std::shared_ptr<GameClient> client;
    std::shared_ptr<LobbyView> _view;
    std::shared_ptr<LobbyController> _controller;
    short _current_option = 0;
    void displayOptions(int mode);
    //void startGame();
    void addPlayer();

public:
    explicit LobbyConsole(const std::string& sessionId, std::shared_ptr<GameClient> client);

    void displayFriends();
    void display() override;
    void displayError() override{}
    void update() override{}
    ReturnInput handleInput() override;
};
