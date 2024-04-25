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

void LobbyController::sendFaction(const std::string& session_id, int faction) {
    nlohmann::json req;
    req["moveType"] = "chooseFaction";
    req["faction"] = faction;
    _game_client->MakeMove(session_id, req).get();
}


void LobbyController::launchGame(const std::string& sessionId) {
  nlohmann::json req;
  req["moveType"] = "StartGame";
  _game_client->MakeMove(sessionId, req).get();
}