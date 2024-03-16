#include "lobby_controller.hh"

LobbyController::LobbyController(std::shared_ptr<GameClient> _game_client)
    : _game_client(_game_client) {}

void LobbyController::sendIDGame(const std::string& destination, const std::string& message) {
    auto userIDFuture = _game_client->GetUserId(destination);

    auto userId = userIDFuture.get();
    if (userId == "")
        std::cout << "pseudo invalide!" << std::endl;
    else {
        std::future<bool> resultFuture = _game_client->SendMessage(userId, message);
        _game_client->AddNotification(destination, "N/A sent you a game request, "  + message.substr(0, message.length()-1));
    }
}


void LobbyController::launchGame(const std::string& sessionId) {
  nlohmann::json req;
  req["session_id"] = sessionId;
  req["move"] = "start";
  _game_client->MakeMove(sessionId, req);
}