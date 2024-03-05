#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <future>

#include "controller.hh"
#include "game_client.hh"

class LoginController{
private:
    std::shared_ptr<GameClient> gameClient;

public:
    explicit LoginController(const std::string& baseUri);
    std::future<bool> attemptLogin(const std::string& username, const std::string& password) const;
    std::future<bool> attemptRegister(const std::string& username, const std::string& password) const;
};
