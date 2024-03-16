#include "main_menu_view.hh"

MainMenuView::MainMenuView(std::shared_ptr<GameClient> gameClient) : _gameClient(gameClient) {}

std::vector<std::string> MainMenuView::getNotifications() {
    std::future<njson> notifsFuture = _gameClient->getNotificationsFromServer();
    njson notifs = notifsFuture.get();
    _notifications.clear();
    for (const auto& notif : notifs) {
        _notifications.push_back(notif);
    }
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
        addFriend(pseudo_to_add, 4);
    }
    return _friends;
}

void MainMenuView::addNotification(const std::string& username, const std::string& new_notification) {
    _gameClient->AddNotification(username, new_notification);
}

void MainMenuView::addFriend(std::string name, int status) {
    std::tuple<std::string, int> new_friend = std::tuple<std::string, int>(name, status);
    _friends.push_back(new_friend);
}

void MainMenuView::update() {
    // Updates friends list and notifications queue (try to keep only recent ones ?)
}
