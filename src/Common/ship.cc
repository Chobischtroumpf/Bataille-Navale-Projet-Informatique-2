#include "ship.hh"

Ship::Ship(std::vector<BoardCoordinates> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() >= _size_x) { _size_x = c.x()+1; }
        if (c.y() >= _size_y) { _size_y = c.y()+1; }
    }
    // _ship_cells(vector<vector<Cell>>(_size_y, vector<Cell>(_size_x))
}

Ship::Ship(std::vector<BoardCoordinates> coordinates, std::shared_ptr<GameView> board): _coordinates(coordinates), _board(std::move(board)) {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() > _size_x) { _size_x = c.x(); }
        if (c.y() > _size_y) { _size_y = c.y(); }
    }
    // _ship_cells(std::vector<std::vector<Cell>>(_size_y, std::vector<Cell>(_size_x))
}

Ship::Ship(const Ship &other): _coordinates(other._coordinates), _top_left(other._top_left), _type(other._type), _board(std::move(other._board)), _length(other._length), _size_x(other._size_x), _size_y(other._size_y), _is_sunk(other._is_sunk) {}

Ship &Ship::operator=(const Ship &other) {
    if (this != &other) {
        _coordinates = other._coordinates;
        _top_left = other._top_left;
        _type = other._type;
        _board = std::move(_board);
        _length = other._length;
        _size_x = other._size_x;
        _size_y = other._size_y;
        _is_sunk = other._is_sunk;
    }
    return *this;
}

bool Ship::operator==(const Ship &other) const {
    return _coordinates == other._coordinates && _top_left == other._top_left && _type == other._type && _length == other._length && _size_x == other._size_x && _size_y == other._size_y && _is_sunk == other._is_sunk;
}

void Ship::rotate() {
    int temp = _size_x;
    _size_x = _size_y;
    _size_y = temp;

    for (auto &c: _coordinates) {
        c.set(-c.y() + _size_x - 1, c.x());
    }
}

std::vector<std::string> Ship::to_string() {
    std::vector<std::string> to_return;
    std::vector<std::vector<std::string>> ship_string(_size_y, std::vector<std::string>(_size_x, "  "));

    for (auto &c: _coordinates) {
        ship_string.at(c.y()).at(c.x()) = "██";
    }

    for (auto &line_string: ship_string) {
        std::string line;
        for (auto &s: line_string) {
            line += s;
        }
        to_return.push_back(line);
    }
    return to_return;
}

std::vector<BoardCoordinates> Ship::getCoordinates() const {
    return _coordinates;
}

BoardCoordinates Ship::getTopLeft() const {
    return _top_left;
}

CellType Ship::getType() const {
    return _type;
}

// const std::vector<std::vector<Cell>> Ship::getShipCells() const {
//     return _ship_cells;
// }

int Ship::getLength() const {
    return _length;
}

int Ship::getSizeX() const {
    return _size_x;
}

int Ship::getSizeY() const {
    return _size_y;
}

bool Ship::isSunk() const {
    return _is_sunk;
}

void Ship::setSunk(bool is_sunk) {
    _is_sunk = is_sunk;
}

// void Ship::setShipCells(const std::vector<std::vector<Cell>> &ship_cells) {
//     _ship_cells = ship_cells;
// }

bool Ship::translate(int x, int y) {
    for (auto &c: _coordinates) {
        c.set(c.x() + x, c.y() + y);
    }
    return true;
}

void Ship::notify(const BoardCoordinates &coords) {
    // (void) coords;
    // // Check if ship is sunk
    // auto board_coordinate = _top_left;

    // if (
    // for (auto &c = _coordinates; c != _coordinates.end(); c++) {
        
    // }


}

void Ship::setType(CellType new_type) {
    _type = new_type;
}