#pragma once

#include <memory>
#include "display.hh"
#include "../common/board_coordinates.hh"
#include "../common/ship_coordinates.hh"


class Controller {
public:
    virtual void setDisplay(std::shared_ptr<Display>) =0;
    virtual void connectServer() =0;
    virtual bool fire(BoardCoordinates) const =0;
    virtual bool placeShip(ShipCoordinates) const =0;
};