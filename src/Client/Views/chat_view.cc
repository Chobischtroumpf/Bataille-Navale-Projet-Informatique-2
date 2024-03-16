#include "chat_view.hh"
#include <iostream>


ChatView::ChatView(std::shared_ptr<GameClient> gameClient)
    : gameClient(gameClient) {}


std::vector<Message> ChatView::getMsgFromServer(const std::string& destination) {
    std::vector<Message> convertedMessages;
    auto userIDFuture = gameClient->GetUserId(destination);
    auto userID = userIDFuture.get();
    auto futureMessages = gameClient->GetMessages(userID);
    auto messagesJson = futureMessages.get();
    for (const auto& jsonMsg : messagesJson) {
        std::string text = jsonMsg["message"];
        std::string sender = jsonMsg["sender"];
        auto userNAMEFuture = gameClient->GetUsername(sender);
        std::string userNAME = userNAMEFuture.get();
        convertedMessages.emplace_back(Message{text,userNAME}); //push_back
    }
    return convertedMessages;
}
