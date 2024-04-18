#include "main_menu_controller.hh"

MainMenuController::MainMenuController(std::shared_ptr<GameClient> _game_client)
    : _game_client(_game_client) {}

bool MainMenuController::createGame(int mode, int friend_id) {
  // Communiquer avec le serveur pour créer une game (normal, commandant) +
  // inviter un ami
}

bool MainMenuController::joinGame(const std::string &game_id) {
  auto resultFuture = _game_client->JoinGame(game_id);
  auto result = resultFuture.get();
  return result.contains("isSuccessful");
}

void MainMenuController::addFriend(const std::string &username) {
  _game_client->AddFriend(username);

  std::future<std::string> resultFuture = _game_client->GetUserId(username);
  std::string result = resultFuture.get();

  if (result != "") {
      _game_client->AddNotification("", "You've just added " + username + " as a friend!");
      _game_client->AddNotification(username,  + "N/A added you as a friend!");
  } else {
      _game_client->AddNotification("",  + "Failed to add " + username + " as a friend!");
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
