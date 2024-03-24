#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include "view.hh"
#include "game_client.hh"
#include <nlohmann/json.hpp>


class LobbyView {
public:
    explicit LobbyView(std::shared_ptr<GameClient> gameClient);
    std::vector<std::string> getUserInGame(const std::string& sessionId);
    bool waitGameStart(const std::string& sessionId);
    nlohmann::json getGameState(const std::string& sessionId);

private:
    std::shared_ptr<GameClient> gameClient; // Client de jeu pour interagir avec le serveur
    nlohmann::json messages; // Stockage des messages récupérés
};
