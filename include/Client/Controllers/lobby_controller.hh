#pragma once

#include <future>
#include <string>

#include "controller.hh"
#include "game_client.hh"

class LobbyController : public Controller {
private:
  std::shared_ptr<GameClient> _game_client;

public:
  explicit LobbyController(std::shared_ptr<GameClient> _game_client);

  void sendIDGame(const std::string &destination, const std::string &message);
};