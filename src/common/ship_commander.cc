#include "../../include/common/ship_commander.hh"

ship_commander::ship_commander(int number_of_case) {
    switch (number_of_case) {
    case 2:
        _ships.push_back(Ship({{0, 0}, {1, 0}}));
        break;
    case 3:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}}));
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}}));
        break;
    case 4:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}, {3, 0}}));
        _ships.push_back(Ship({{0, 1}, {1, 0}, {1, 1}, {2, 0}}));
        _ships.push_back(Ship({{0, 0}, {1, 0}, {1, 1}, {2, 1}}));
        _ships.push_back(Ship({{0, 1}, {1, 1}, {2, 0}, {2, 1}}));
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}, {2, 1}}));
        _ships.push_back(Ship({{0, 1}, {1, 0}, {1, 1}, {2, 1}}));
        break;
    case 5:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}));
        _ships.push_back(Ship({{0, 0}, {0, 1}, {1, 1}, {2, 0}, {2, 1}}));
        break;
    default:
        break;
    }
}

void ship_commander::next() {
    _pos ++;
    _pos %= _ships.size();
}

Ship ship_commander::getShip() {
    return _ships[_pos];
}

void ship_commander::rotate() {
    _ships[_pos].rotate();
}