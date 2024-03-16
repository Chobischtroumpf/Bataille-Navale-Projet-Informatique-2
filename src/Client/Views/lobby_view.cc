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

bool LobbyView::isGameStarted(const std::string& sessionId){
    auto futureMessages = gameClient->QueryGameState(sessionId);
    auto messagesJson = futureMessages.get();
    // return if the game has started
    return false;
}