#include "main_menu_controller.hh"


MainMenuController::MainMenuController(std::shared_ptr<GameClient> client): gameClient(client) {}

bool MainMenuController::createGame(int mode, int friend_id) {
    // Communiquer avec le serveur pour créer une game (normal, commandant) + inviter un ami
}

void MainMenuController::joinGame(int game_id) {
    // Communiquer avec le serveur pour rejoindre une game à l'aide d'une game_id
}

void MainMenuController::addFriend(const std::string& username) {
    std::future<bool> resultFuture = gameClient->AddFriend(username);
    // This code will not work due to the missing "error" section in each response
    bool result = resultFuture.get();
    if (result) {
        std::cout << "Friend request sent." << std::endl;
        //std::cout << "Friend added!" << std::endl;
    }
    else {
        std::cout << "Failed to add friend :(" << std::endl;
    }
}
