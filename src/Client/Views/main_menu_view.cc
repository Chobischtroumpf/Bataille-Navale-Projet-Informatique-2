#include "main_menu_view.hh"

MainMenuView::MainMenuView(std::shared_ptr<GameClient> gameClient) : _gameClient(gameClient) {}

std::vector<std::string> MainMenuView::getNotifications() const {
    return _notifications;
}

std::vector<std::tuple<std::string, int>> MainMenuView::getFriends() {
    std::future<njson> friendlistFuture = _gameClient->GetFriends();
    njson friendlist = friendlistFuture.get();
    _friends.clear();
    for (const auto& userId : friendlist) {
        std::string Id = userId[0];
        auto usernameFuture = _gameClient->GetUsername(Id);
        auto username = usernameFuture.get();
        std::string pseudo_to_add(username);
        addFriend(pseudo_to_add, 0);
    }
    return _friends;
}

void MainMenuView::addNotification(std::string new_notification) {
    _notifications.push_back(new_notification);
}

void MainMenuView::addFriend(std::string name, int status) {
    std::tuple<std::string, int> new_friend = std::tuple<std::string, int>(name, status);
    _friends.push_back(new_friend);
}

void MainMenuView::update() {
    // Updates friends list and notifications queue (try to keep only recent ones ?)
}
