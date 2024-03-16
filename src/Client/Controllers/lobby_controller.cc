#include "lobby_controller.hh"

LobbyController::LobbyController(std::shared_ptr<GameClient> gameClient)
    : _game_client(gameClient) {}

void LobbyController::sendIDGame(const std::string& destination, const std::string& message) {
    auto userIDFuture = gameClient->GetUserId(destination);
    auto userId = userIDFuture.get();
    std::cout << "ID du joueur invalide : " << userId << std::endl;
    if (userId == "")
        std::cout << "pseudo invalide!" << std::endl;
    else
        std::future<bool> resultFuture = gameClient->SendMessage(userId, message);
    auto userIDFuture = _game_client->GetUserId(destination);
    auto userID = userIDFuture.get();

    std::future<bool> resultFuture = _game_client->SendMessage(userID, message);
}

