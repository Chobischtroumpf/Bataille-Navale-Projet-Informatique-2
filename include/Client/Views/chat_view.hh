#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include "view.hh"
#include "message.hh"
#include "game_client.hh"
#include <nlohmann/json.hpp>


class ChatView {
public:
    explicit ChatView(std::shared_ptr<GameClient> gameClient);
    std::vector<Message> getMsgFromServer(const std::string& destination);

private:
    std::shared_ptr<GameClient> gameClient; // Client de jeu pour interagir avec le serveur
    std::string recipientId; // Identifiant du destinataire pour la récupération des messages
    nlohmann::json messages; // Stockage des messages récupérés
    std::vector<Message> messagesClasse;
    std::vector<Message> convertJsonToMessages();
};
