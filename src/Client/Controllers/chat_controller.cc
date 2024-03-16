#include <fstream>
#include <iostream>

#include "chat_controller.hh"

ChatController::ChatController(std::shared_ptr<GameClient> gameClient)
    : _game_client(gameClient) {}

void ChatController::sendMsgToServer(const std::string &destination,
                                     const std::string &message) {
  auto userIDFuture = _game_client->GetUserId(destination);
  auto userID = userIDFuture.get();
  std::cout << userID << std::endl;
  std::future<bool> resultFuture = _game_client->SendMessage(userID, message);
  bool result = resultFuture.get();
  if (result) {
    std::cout << "Message send to server" << std::endl;
  } else {
    std::cout << "Fail to send message to server" << std::endl;
  }
}
