#include "Common/ship.hh"

Ship::Ship(std::vector<BoardCoordinates> coordinates, std::shared_ptr<GameView> board): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() > _size_x) { _size_x = c.x(); }
        if (c.y() > _size_y) { _size_y = c.y(); }
    }
    // _ship_cells(vector<vector<Cell>>(_size_y, vector<Cell>(_size_x))
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
    vector<vector<std::string>> to_print(_size_y, vector<std::string>(_size_x, "  "));

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

vector<BoardCoordinates> Ship::getCoordinates() {
    return _coordinates;
}

BoardCoordinates Ship::getTopLeft() {
    return _top_left;
}

// const vector<vector<Cell>> Ship::getShipCells() const {
//     return _ship_cells;
// }

int Ship::getLength() {
    return _length;
}

bool Ship::isSunk() const {
    return _is_sunk;
}

void Ship::setSunk(bool is_sunk) {
    _is_sunk = is_sunk;
}

// void Ship::setShipCells(const vector<vector<Cell>> &ship_cells) {
//     _ship_cells = ship_cells;
// }

void Ship::notify(const BoardCoordinates &coords) {

    // Check if ship is sunk

  }

void Ship::setType(CellType new_type) {
    _type = new_type;
}