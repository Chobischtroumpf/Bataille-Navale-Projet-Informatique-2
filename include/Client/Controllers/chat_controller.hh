#pragma once

#include <future>
#include <memory>
#include <string>

#include "controller.hh"
#include "game_client.hh"
#include "message.hh"

class ChatController : public Controller {
private:
  std::shared_ptr<GameClient> _game_client;

public:
  void sendMsgToServer(const std::string &destination,
                       const std::string &message);
  explicit ChatController(std::shared_ptr<GameClient> gameClients);
};
