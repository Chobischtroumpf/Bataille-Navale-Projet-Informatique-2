#include "lobby_controller.hh"

LobbyController::LobbyController(std::shared_ptr<GameClient> gameClient)
    : _game_client(gameClient) {}

void LobbyController::sendIDGame(const std::string& destination, const std::string& message) {
    auto userIDFuture = _game_client->GetUserId(destination);
    auto userID = userIDFuture.get();

    std::future<bool> resultFuture = _game_client->SendMessage(userID, message);
}

void LobbyController::launchGame(const std::string& sessionId) {
  nlohmann::json req;
  req["session_id"] = sessionId;
  req["move"] = "start";
  _game_client->MakeMove(sessionId, req);
}