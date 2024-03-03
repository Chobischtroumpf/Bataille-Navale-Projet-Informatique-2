#pragma once

#include <string>
#include <memory>
#include "../controller.hh"
#include "../network/game_client.hh"
#include <future>


class LoginController{
private:
    std::shared_ptr<GameClient> gameClient;

public:
    explicit LoginController(const std::string& baseUri);
    std::future<bool> attemptLogin(const std::string& username, const std::string& password) const;
    std::future<bool> attemptRegister(const std::string& username, const std::string& password) const;
};
