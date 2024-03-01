#pragma once

#include "player.hh"

/*
 * Interface qui permet de mettre en place l'observer subject/pattern
*/
class Subject {
public:
  virtual void registerObserver(Player) = 0;
  virtual void unregisterObserver(Player) = 0;
  virtual void notify_observers() const = 0;
};