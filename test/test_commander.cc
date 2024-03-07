#include <cstddef>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <optional>
#include <stdexcept>

/*
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
*/

/*
is_ship
is_mine
is_kwown
is_hit
is_sunk

plateau adverse             - plateau joueur
eau non decouverte          - eau                     - 0
eau decouverte ou scanné  ╳ - eau scanné ou touché  ╳ - is_knwon

None                        - case mine             ¤ - is_mine
case mine scanné          █ - case mine             ¤ - is_mine | is_kwown
case mine touché          ¤ - case mine touché      * - is_mine | is_hit

None                        - case bateau           █ - is_ship
case bateau scanné        █ - case bateau scanné    █ - is_ship | is_kwown
case bateau touché        ▒ - case bateau touché    ▒ - is_ship | is_hit
case bateau coulé         ░ - case bateau coulé     ░ - is_ship | is_hit | is_sunk
*/

typedef enum {
  // Flags:
  IS_SHIP = 0b00001,
  IS_MINE = 0b00010,
  IS_KNOWN = 0b00100,
  IS_HIT = 0b01000,
  IS_SUNK = 0b10000,

  // Non-ship types:
  WATER = 0,        //< water (my side) or unknown (assumed water, their side)
  OCEAN = IS_KNOWN, //< was empty target

  // Mine:
  MINE = IS_MINE,
  SCANNED_MINE = IS_MINE | IS_KNOWN,
  HIT_MINE = IS_MINE | IS_HIT,

  // Ship states:
  UNDAMAGED_SHIP = IS_SHIP,
  SCANNED_SHIP = IS_SHIP | IS_KNOWN,
  HIT_SHIP = IS_SHIP | IS_HIT,
  SUNK_SHIP = IS_SHIP | IS_HIT | IS_SUNK,
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
        case UNDAMAGED_SHIP:
            return "█ ";
        case HIT_SHIP:
            return "▒ ";
        case SUNK_SHIP:
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
    Ship(std::vector<std::pair<int, int>> coordinates);
    void rotate();
    void print();
    std::vector<std::pair<int, int>> getCoordinates();
};

Ship::Ship(std::vector<std::pair<int, int>> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        if (c.first + 1 > _size_x) { _size_x = c.first + 1; }
        if (c.second + 1 > _size_y) { _size_y = c.second + 1; }
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

std::vector<std::pair<int, int>> Ship::getCoordinates() {
    return _coordinates;
}


class ShipClassic {
protected:
    std::vector<Ship> _ships;
    int _pos = 0;
public:
    ShipClassic(int number_of_case);
    void next();
    Ship getShip();
    void rotate();
};

ShipClassic::ShipClassic(int number_of_case) {
    switch (number_of_case) {
      case 2:
        _ships.push_back(Ship({{0, 0}, {1, 0}}));
        break;
      case 3:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}}));
        break;
      case 4:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}, {3, 0}}));
        break;
      case 5:
        _ships.push_back(Ship({{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}));
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


class ShipCommander : public ShipClassic {
public:
    ShipCommander(int number_of_case);
};

ShipCommander::ShipCommander(int number_of_case): ShipClassic{number_of_case} {
    switch (number_of_case) {
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
        _my_board[y + c.second][x + c.first].setType(UNDAMAGED_SHIP);
    }
  }


int main() {
	LocalBoard test;
    ShipClassic* ships = new ShipCommander(4);
    //ships->next();
    //ships->next();
    //ships->next();
    //ships->next();
    //ships->next();
    //ships->next();
    //ships->rotate();
    //ships->rotate();
    //ships->rotate();
    //ships->rotate();
    ships->getShip().print();
    test.addShip(ships->getShip(), 2, 2);
    test.print();
    delete ships;
    return 0;
}
/*
¤
?

█ ¤ █
█ █ █

? ? ?
? ? ?
*/