#include "../../../include/client/Views/main_menu_view.hh"

std::vector<std::string> MainMenuView::getNotifications() const {
    return _notifications;
}

std::vector<std::tuple<std::string, int>> MainMenuView::getFriends() const {
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
