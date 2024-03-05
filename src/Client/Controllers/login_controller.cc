#include "login_controller.hh"

LoginController::LoginController(const std::string& baseUri) 
    : gameClient(std::make_shared<GameClient>(baseUri)) {}

std::future<bool> LoginController::attemptLogin(const std::string& username, const std::string& password) const {
    return gameClient->Login(username, password);
}

std::future<bool> LoginController::attemptRegister(const std::string& username, const std::string& password) const {
    return gameClient->Register(username, password);
}