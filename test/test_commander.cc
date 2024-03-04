#include <cstddef>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <optional>
#include <stdexcept>

typedef enum {
  // Flags:
  IS_SHIP = 0b001,
  IS_KNOWN = 0b010, //< was a target
  IS_SUNK = 0b100,

  // Non-ship types:
  WATER = 0,        //< water (my side) or unknown (assumed water, their side)
  OCEAN = IS_KNOWN, //< was empty target

  // Ship states:
  UNDAMAGED = IS_SHIP,                 //< undamaged ship, used for my side
  HIT = IS_SHIP | IS_KNOWN,            //< hit ship
  SUNK = IS_SHIP | IS_KNOWN | IS_SUNK, //< sunk ship
} CellType;

class Cell {
  CellType _type{WATER};
  std::optional<int> _ship_id{std::nullopt};

public:
  /** Default constructor: creates an unkown/Water cell */
  constexpr Cell() = default;
  // Public method to set the type of the cell
  void setType(CellType newType) { _type = newType; }
  [[nodiscard]] constexpr inline CellType type() const { return _type; }
  std::string toString() {
    switch (_type) {
        case WATER:
            return "- ";
        case OCEAN:
            return "╳ ";
        case UNDAMAGED:
            return "█ ";
        case HIT:
            return "▒ ";
        case SUNK:
            return "░ ";
    }
  }
};

class Ship {
private:
    std::vector<std::pair<int, int>> _coordinates;
    int _size_x = 0;
    int _size_y = 0;

public:
    Ship() {};
    Ship(std::vector<std::pair<int, int>> coordinates): _coordinates(coordinates) {
        for (auto &c: coordinates) {
            if (c.first + 1 > _size_x) { _size_x = c.first + 1; }
            if (c.second + 1 > _size_y) { _size_y = c.second + 1; }
        }
    };
    void addCoordinate(int x, int y);
    void rotate();
    std::vector<std::pair<int, int>> getCoordinates();
};

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


class LocalBoard {
public:
  LocalBoard();
  void print();
  void addShip(Ship s, int x, int y);

private:
  std::vector<std::vector<Cell>> _my_board;
  std::vector<std::vector<Cell>> _their_board;
  bool _my_turn;
  bool _is_finished;
  bool _is_victory;
};

LocalBoard::LocalBoard()
    : _is_finished{false},
      _is_victory{false},
      _my_turn{true},
      _my_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))},
      _their_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))} {}

void LocalBoard::print() {
    for (auto &ligne: _my_board) {
        for (auto &c: ligne) {
            std::cout << c.toString();
        }
        std::cout << std::endl;
    }
  }

void LocalBoard::addShip(Ship s, int x, int y) {
    for (auto &c: s.getCoordinates()) {
        _my_board[y + c.second][x + c.first].setType(UNDAMAGED);
    }
  }


int main() {
	LocalBoard test;
    ship_commander ships(5);
    //ships.next();
    //ships.next();
    //ships.next();
    //ships.next();
    //ships.next();
    //ships.next();
    //ships.rotate();
    //ships.rotate();
    //ships.rotate();
    //ships.rotate();
    test.addShip(ships.getShip(), 2, 2);
    test.print();
    return 0;
}
