#include "login_controller.hh"

LoginController::LoginController(std::shared_ptr<GameClient> gameClient)
    : _game_client(gameClient) {}

std::future<bool> LoginController::attemptLogin(const std::string &username,
                              const std::string &password) const {
  return _game_client->Login(username, password);
}

std::future<bool> LoginController::attemptRegister(const std::string &username,
                                 const std::string &password) const {
  return _game_client->Register(username, password);
}

std::future<bool> LoginController::addNotification(const std::string &username,
                                  const std::string &new_notification) {
  return _game_client->AddNotification(username, new_notification);
}