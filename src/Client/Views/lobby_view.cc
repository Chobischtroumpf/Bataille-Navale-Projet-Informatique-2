#include "lobby_view.hh"

LobbyView::LobbyView(std::shared_ptr<GameClient> gameClient)
    : gameClient(gameClient) {}


std::vector<std::string> LobbyView::getUserInGame(const std::string& sessionId){
    std::vector<std::string> convertedMessages;
    auto futureMessages = gameClient->QueryGameState(sessionId);
    auto messagesJson = futureMessages.get();
    auto usersID = messagesJson["participants"];
    for (const auto& userId : usersID) {
        auto futureName = gameClient->GetUsername(userId);
        auto name = futureName.get();
        convertedMessages.emplace_back(name);
    }
    return convertedMessages;
}

bool LobbyView::waitGameStart(const std::string& sessionId){
    bool started = false;
    while (!started) {
      auto futureMessages = gameClient->QueryGameState(sessionId);
      auto messagesJson = futureMessages.get();
      if (messagesJson.at("hasStarted") == true) started = true;
      sleep(1);
    }
    return true;
}

nlohmann::json LobbyView::getGameState(const std::string& session_id){
    auto futureMessages = gameClient->QueryGameState(session_id);
    auto messagesJson = futureMessages.get();
    return messagesJson;
}