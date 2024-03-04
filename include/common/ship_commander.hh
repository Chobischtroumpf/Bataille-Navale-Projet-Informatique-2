#pragma once

#include "ship.hh"
#include "not_implemented_error.hh"
#include <optional>
#include <string>
#include <vector>

class ship_commander {
private:
    std::vector<Ship> _ships;
    int _pos = 0;
public:
    ship_commander(int number_of_case);
    void next();
    Ship getShip();
    void rotate();
};