#pragma once

#include "ship.hh"
#include "not_implemented_error.hh"
#include <optional>
#include <string>
#include <vector>

class ShipCommander {
private:
    std::vector<Ship> _ships;
    int _pos = 0;
public:
    ShipCommander(int number_of_case);
    void next();
    Ship getShip();
    void rotate();
};