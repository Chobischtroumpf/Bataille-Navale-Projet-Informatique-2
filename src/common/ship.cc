#include "../../include/common/ship.hh"

Ship::Ship(std::vector<std::pair<int, int>> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        if (c.first > _size_x) { _size_x = c.first; }
        if (c.second > _size_y) { _size_y = c.second; }
    }
}

void Ship::addCoordinate(int x, int y) {
    _coordinates.push_back({x, y});
    if (x + 1 > _size_x) { _size_x = x + 1; }
    if (y + 1 > _size_y) { _size_y = y + 1; }
}

void Ship::rotate() {
    int temp = _size_x;
    _size_x = _size_y;
    _size_y = temp;

    for (auto &c: _coordinates) {
        int temp = c.first;
        c.first = -c.second + _size_x - 1;
        c.second = temp;
    }
}

std::vector<std::pair<int, int>> Ship::getCoordinates() {
    return _coordinates;
}