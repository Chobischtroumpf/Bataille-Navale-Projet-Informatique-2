#include "ship.hh"

Ship::Ship(std::vector<BoardCoordinates> coordinates): _coordinates{coordinates} {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() >= _size_x) { _size_x = c.x()+1; }
        if (c.y() >= _size_y) { _size_y = c.y()+1; }
    }
    if (_length == 1) setType(UNDAMAGED_MINE);
    // _ship_cells(vector<vector<Cell>>(_size_y, vector<Cell>(_size_x))
}
Ship::Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates, bool _side):
                        _coordinates{coordinates}, _top_left{top_left}, _side{_side} {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() >= _size_x) { _size_x = c.x()+1; }
        if (c.y() >= _size_y) { _size_y = c.y()+1; }
    }
    if (_length == 1) setType(UNDAMAGED_MINE);
    // _ship_cells(vector<vector<Cell>>(_size_y, vector<Cell>(_size_x))
}

Ship::Ship(std::vector<BoardCoordinates> coordinates, GameView *board): _coordinates{coordinates}, _board{board} {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() >= _size_x) { _size_x = c.x()+1; }
        if (c.y() >= _size_y) { _size_y = c.y()+1; }
    }
    if (_length == 1) setType(UNDAMAGED_MINE);
    // _ship_cells(std::vector<std::vector<Cell>>(_size_y, std::vector<Cell>(_size_x))
}

Ship::Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates, GameView *board, bool _side):
                                _coordinates{coordinates}, _top_left{top_left}, _board{board}, _side{_side} {
    for (auto &c: coordinates) {
        _length++;
        if (c.x() >= _size_x) { _size_x = c.x()+1; }
        if (c.y() >= _size_y) { _size_y = c.y()+1; }
    }
    if (_length == 1) setType(UNDAMAGED_MINE);
}

Ship::Ship(const Ship &other): _coordinates(other.getCoordinates()),
            _top_left{other.getTopLeft()}, _type{other.getType()},
            _board{other.getBoard()}, _length{other.getLength()},
            _size_x{other.getSizeX()}, _size_y{other.getSizeY()},
            _is_sunk{other.isSunk()}, _side{other.getSide()} {
}

Ship &Ship::operator=(const Ship &other) {
    if (this != &other) {
        _coordinates = other._coordinates;
        _top_left = other._top_left;
        _type = other._type;
        _board = other.getBoard();
        _length = other.getLength();
        _size_x = other.getSizeX();
        _size_y = other.getSizeY();
        _is_sunk = other.isSunk();
    }
    return *this;
}

bool Ship::operator==(const Ship &other) const {
    // if (this && &other == nullptr) return false;
    // if (this == nullptr && &other) return false;
    // if (this == nullptr && other == nullptr) return true;
    return _coordinates == other._coordinates && _top_left == other._top_left && _type == other._type && _length == other._length && _size_x == other._size_x && _size_y == other._size_y && _is_sunk == other._is_sunk;
}

bool Ship::operator==(const Ship *other) const {
    if (this && other == nullptr) return false;
    if (this == nullptr && other) return false;
    if (this == nullptr && other == nullptr) return true;
    return _coordinates == other->_coordinates && _top_left == other->_top_left && _type == other->_type && _length == other->_length && _size_x == other->_size_x && _size_y == other->_size_y && _is_sunk == other->_is_sunk;
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

nlohmann::json Ship::to_json() {
    nlohmann::json j;
    j["anchor"] = _top_left.to_json();
    j["coordinates"] = nlohmann::json::array();
    for (auto &c: _coordinates) {
        j["coordinates"].push_back(c.to_json());
    }
    j["type"] = _type;
    return j;
}

std::vector<BoardCoordinates> Ship::getCoordinates() const {
    return _coordinates;
}

bool Ship::isAt(const BoardCoordinates &coords) const {
    for (auto &c: _coordinates) {
        if ((c + getTopLeft()) == coords) {
            return true;
        }
    }
    return false;
}

BoardCoordinates Ship::getTopLeft() const {
    return _top_left;
}

CellType Ship::getType() const {
    return _type;
}

GameView *Ship::getBoard() const {
    return _board;

}

size_t Ship::getLength() const {
    return _length;
}

size_t Ship::getSizeX() const {
    return _size_x;
}

size_t Ship::getSizeY() const {
    return _size_y;
}

bool Ship::isSunk() const {
    return _is_sunk;
}

bool Ship::getSide() const {
    return _side;
}

void Ship::setSunk(bool is_sunk) {
    _is_sunk = is_sunk;
}

void Ship::setTopLeft(BoardCoordinates top_left) {
    _top_left = top_left;
}

void Ship::setBoard(GameView *board) {
    _board = board;
}

bool Ship::translate(int x, int y) {
    for (auto &c: _coordinates) {
        c.set(c.x() + x, c.y() + y);
    }
    return true;
}

void Ship::notify() {
    std::clog << "SHIP::NOTIFY" << std::endl;
    // Check if ship is sunk
    for (auto &c: _coordinates) {
        if (_board->cellType( _side, _top_left+c) != HIT_SHIP) {
            std::clog << "SHIP::NOTIFY::RETURN" << std::endl;
            return;
        }
    }
    std::clog << "SHIP::NOTIFY::SUNK" << std::endl;
    setSunk(true);
}

void Ship::setType(CellType new_type) {
    _type = new_type;
}

bool Ship::isPartOfShip(BoardCoordinates coord) {
    for (auto &c: _coordinates) {
        if (_top_left + c == coord) {
            return true;
        }
    }
    return false;
}

Ship::~Ship() {
    _board = nullptr;
}