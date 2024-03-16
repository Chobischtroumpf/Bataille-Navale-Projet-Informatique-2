#include "lobby_controller.hh"

LobbyController::LobbyController(std::shared_ptr<GameClient> gameClient)
    : gameClient(gameClient) {}

void LobbyController::sendIDGame(const std::string& destination, const std::string& message) {
    auto userIDFuture = gameClient->GetUserId(destination);
    auto userID = userIDFuture.get();

    std::future<bool> resultFuture = gameClient->SendMessage(userID, message);
}