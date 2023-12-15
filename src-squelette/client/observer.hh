#pragma once

/**
 * Interface des classes observers, qui oberserveront un sujet.
 */
class Observer {
public:
  virtual void update() = 0;
};