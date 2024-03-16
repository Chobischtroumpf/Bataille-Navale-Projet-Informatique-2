#include "ship_commander.hh"

ShipCommander::ShipCommander(int number_of_case): ShipClassic(number_of_case) {
    switch (number_of_case) {
      case 1:
        _ships.push_back(Ship({{0, 0}}));
        _ships[1].setType(UNDAMAGED_MINE);
        break;
      case 3:
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}}));
        break;
      case 4:
        _ships.push_back(Ship({{0, 1}, {1, 0}, {1, 1}, {2, 0}}));
        _ships.push_back(Ship({{0, 0}, {1, 0}, {1, 1}, {2, 1}}));
        _ships.push_back(Ship({{0, 1}, {1, 1}, {2, 0}, {2, 1}}));
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}, {2, 1}}));
        _ships.push_back(Ship({{0, 1}, {1, 0}, {1, 1}, {2, 1}}));
        break;
      case 5:
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}, {2, 0}, {2, 1}}));
        break;
      default:
        break;
    }
}