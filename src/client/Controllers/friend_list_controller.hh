#pragma once

#include "../controller.hh"

class FriendListController : public Controller {
public:
    void add_friend(std::string user_name);
    void delete_friend(std::string user_name);
private:

};