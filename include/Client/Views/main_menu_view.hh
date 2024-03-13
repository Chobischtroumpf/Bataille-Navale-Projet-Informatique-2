#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "view.hh"

#include "game_client.hh"

class MainMenuView: public View {
public:
    MainMenuView(std::shared_ptr<GameClient> gameClient);
    std::vector<std::string> getNotifications() const;
    std::vector<std::tuple<std::string, int>> getFriends() const;
    void addNotification(std::string new_notification);
    void addFriend(std::string name, int status);
    void update() override;
private:
    std::vector<std::string> _notifications;
    std::vector<std::tuple<std::string, int>> _friends;
    std::shared_ptr<GameClient> _gameClient;
};