#include "main_menu_controller.hh"

MainMenuController::MainMenuController(std::shared_ptr<GameClient> _game_client)
    : _game_client(_game_client) {}

bool MainMenuController::createGame(int mode, int friend_id) {
  // Communiquer avec le serveur pour créer une game (normal, commandant) +
  // inviter un ami
}

void MainMenuController::joinGame(const std::string &game_id) {
  auto resultFuture = _game_client->JoinGame(game_id);
}

void MainMenuController::addFriend(const std::string &username) {
  std::future<bool> resultFuture = _game_client->AddFriend(username);
  // This code will not work due to the missing "error" section in each response
  bool result = resultFuture.get();
  if (result) {
    std::cout << "Friend request sent." << std::endl;
    // std::cout << "Friend added!" << std::endl;
  } else {
    std::cout << "Failed to add friend :(" << std::endl;
  }
}

bool MainMenuController::validUser(const std::string& username){
    auto resultFuture = _game_client->GetUserId(username);
    auto result = resultFuture.get();
    if (result == "")
        return false;
    else
        return true;
}
