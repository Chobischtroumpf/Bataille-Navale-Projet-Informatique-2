#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "view.hh"

class MainMenuView: public View {
public:
    std::vector<std::string> getNotifications() const;
    std::vector<std::tuple<std::string, int>> getFriends() const;
    void addNotification(std::string new_notification);
    void addFriend(std::string name, int status);
    void update() override;
private:
    std::vector<std::string> _notifications;
    std::vector<std::tuple<std::string, int>> _friends;
};