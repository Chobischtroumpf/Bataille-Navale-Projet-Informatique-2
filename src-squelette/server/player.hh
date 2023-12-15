#pragma once

#include "connected_user.hh"
#include <memory>

class Player {
public:
  Player();

private:
    std::shared_ptr<ConnectedUser> _user;
};