#pragma once

#include <future>
#include <iostream>
#include <memory>
#include <string>

#include "controller.hh"
#include "game_client.hh"

class LoginController {
private:
  std::shared_ptr<GameClient> _game_client;

public:
  explicit LoginController(std::shared_ptr<GameClient> gameClient);
  std::future<bool> attemptLogin(const std::string &username,
                                 const std::string &password) const;
  std::future<bool> attemptRegister(const std::string &username,
                                    const std::string &password) const;
};
