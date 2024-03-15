#include "login_controller.hh"

LoginController::LoginController(std::shared_ptr<GameClient> gameClient)
    : gameClient(gameClient) {}

std::future<bool> LoginController::attemptLogin(const std::string& username, const std::string& password) const {
    return gameClient->Login(username, password);
}

std::future<bool> LoginController::attemptRegister(const std::string& username, const std::string& password) const {
    return gameClient->Register(username, password);
}