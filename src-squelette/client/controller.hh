#pragma once

#include "../common/board_coordinates.hh"
#include "../common/ship_coordinates.hh"
#include "display.hh"
#include <memory>

/*
 * Interface du Controller (ce qui permet d'envoyer des données)
 */
class Controller {
public:
  virtual void setDisplay(std::shared_ptr<Display>) = 0;
  virtual void connectServer() = 0;
  virtual bool fire(BoardCoordinates) const = 0;
  virtual bool placeShip(ShipCoordinates) const = 0;
};