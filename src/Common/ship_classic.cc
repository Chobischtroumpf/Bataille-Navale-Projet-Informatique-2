#include "ship_classic.hh"
ShipClassic::ShipClassic(int number_of_case) {
    switch (number_of_case) {
    case 2:
        _ships.push_back(Ship(std::vector<BoardCoordinates>({{0, 0}, {1, 0}})));
        break;
    case 3:
        _ships.push_back(Ship(std::vector<BoardCoordinates>({{0, 0}, {1, 0}, {2, 0}})));
        break;
    case 4:
        _ships.push_back(Ship(std::vector<BoardCoordinates>({{0, 0}, {1, 0}, {2, 0}, {3, 0}})));
        break;
    case 5:
        _ships.push_back(Ship(std::vector<BoardCoordinates>({{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}})));
        break;
    default:
        break;
    }
}

void ShipClassic::next() {
    _pos ++;
    _pos %= _ships.size();
}

Ship ShipClassic::getShip() {
    return _ships[_pos];
}

void ShipClassic::rotate() {
    _ships[_pos].rotate();
}