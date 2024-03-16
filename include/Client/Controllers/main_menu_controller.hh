#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <future>
#include <string>

#include "controller.hh"
#include "game_client.hh"

class MainMenuController : public Controller {
private:
    std::shared_ptr<GameClient> gameClient;

public:
    explicit MainMenuController(std::shared_ptr<GameClient> client);
    bool createGame(int mode, int friend_id);
    void joinGame(const std::string& game_id);
    void addFriend(const std::string& username);
    void manageProfile();
    void quit();
};