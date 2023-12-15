#pragma once

#include "player.hh"

class Subject {
public:
  virtual void registerObserver(Player) = 0;
  virtual void unregisterObserver(Player) = 0;
  virtual void notify_observers() const = 0;
};