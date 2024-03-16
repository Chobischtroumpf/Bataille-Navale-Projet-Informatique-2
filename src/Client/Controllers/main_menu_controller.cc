#include "main_menu_controller.hh"

MainMenuController::MainMenuController(std::shared_ptr<GameClient> client)
    : _game_client(client) {}

bool MainMenuController::createGame(int mode, int friend_id) {
  // Communiquer avec le serveur pour créer une game (normal, commandant) +
  // inviter un ami
}

void MainMenuController::joinGame(const std::string &game_id) {
  auto resultFuture = _game_client->JoinGame(game_id);
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
