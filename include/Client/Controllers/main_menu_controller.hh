#pragma once

#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "controller.hh"
#include "game_client.hh"

class MainMenuController : public Controller {
private:
  std::shared_ptr<GameClient> _game_client;

public:
    explicit MainMenuController(std::shared_ptr<GameClient> client);
    bool validUser(const std::string& username);
    bool createGame(int mode, int friend_id);
    bool joinGame(const std::string& game_id);
    void addFriend(const std::string& username);
    void manageProfile();
    void quit();
};