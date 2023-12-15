#pragma once

#include "connected_user.hh"
#include "server.hh"
#include <vector>

/*
 * Permet de gérer une partie
 */
class GameServer : public Server {
public:
  bool notify_observers();

private:
  std::vector<ConnectedUser> observers;
};