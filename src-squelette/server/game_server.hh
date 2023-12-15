#pragma once

#include "connected_user.hh"
#include "server.hh"
#include <vector>

class GameServer : public Server {
public:
  bool notify_observers();

private:
  std::vector<ConnectedUser> observers;
};