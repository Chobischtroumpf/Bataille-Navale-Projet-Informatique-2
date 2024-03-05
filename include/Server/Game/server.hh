#pragma once

#include <vector>

#include "player.hh"
#include "subject.hh"

/*
 * Serveur général du jeu
 */
class Server : public Subject {
public:
  virtual void registerObserver(Player);
  virtual void unregisterObserver(Player);
  virtual void notify_observers() const;
  bool isConnected(unsigned int user_id) const;

private:
  std::vector<ConnectedUser> observers;
};