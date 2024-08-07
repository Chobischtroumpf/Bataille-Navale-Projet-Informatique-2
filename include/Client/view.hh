#pragma once

#include "observer.hh"

/*
 * Interface de la vue (ce qui est représenté à l'écran)
 */
class View : public Observer {
public:
  virtual ~View() override {}
  virtual void update() override{}

};