#pragma once

#include "controller.hh"

class FriendListController : public Controller {
public:
    void addFriend(std::string user_name);
    void deleteFriend(std::string user_name);
private:

};
