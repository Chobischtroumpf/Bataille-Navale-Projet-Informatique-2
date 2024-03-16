#pragma once

#include <string>
#include <future>

#include "game_client.hh"
#include "controller.hh"

class LobbyController : public Controller {
private:
    std::shared_ptr<GameClient> gameClient;

public:
    explicit LobbyController(std::shared_ptr<GameClient> gameClient);

    void sendIDGame(const std::string& destination, const std::string& message);
};