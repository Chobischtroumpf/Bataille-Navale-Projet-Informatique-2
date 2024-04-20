#pragma once

#include <vector>

#include "ship.hh"

class ShipClassic {
  protected:
    std::vector<Ship> _ships;
    int _pos = 0;
  public:
    ShipClassic(int number_of_case);
    void next();
    Ship getShip();
    void rotate();

    virtual ~ShipClassic() = default;
};