#include "../../include/common/ship.hh"

Ship::Ship(std::vector<std::pair<int, int>> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        if (c.first > _size_x) { _size_x = c.first; }
        if (c.second > _size_y) { _size_y = c.second; }
    }
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

void Ship::print() {
    std::vector<std::vector<std::string>> to_print(_size_y, std::vector<std::string>(_size_x, "  "));

    for (auto &c: _coordinates) {
        to_print[c.second][c.first] = "██";
    }

    for (auto &to_print2: to_print) {
        for (auto &p: to_print2) {
            std::cout << p;
        }
        std::cout << std::endl;
    }
}