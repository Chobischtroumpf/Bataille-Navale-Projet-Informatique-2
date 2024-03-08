#include "../../include/common/ship.hh"

Ship::Ship(std::vector<BoardCoordinates> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        _number_of_case++;
        if (c.x() > _size_x) { _size_x = c.x(); }
        if (c.y() > _size_y) { _size_y = c.y(); }
    }
}

void Ship::rotate() {
    int temp = _size_x;
    _size_x = _size_y;
    _size_y = temp;

    for (auto &c: _coordinates) {
        c.set(-c.y() + _size_x - 1, c.x());
    }
}

void Ship::print() {
    std::vector<std::vector<std::string>> to_print(_size_y, std::vector<std::string>(_size_x, "  "));

    for (auto &c: _coordinates) {
        to_print[c.y()][c.x()] = "██";
    }

    for (auto &to_print2: to_print) {
        for (auto &p: to_print2) {
            std::cout << p;
        }
        std::cout << std::endl;
    }
}

std::vector<BoardCoordinates> Ship::getCoordinates() {
    return _coordinates;
}

int Ship::getNumberOfCase() {
    return _number_of_case;
}