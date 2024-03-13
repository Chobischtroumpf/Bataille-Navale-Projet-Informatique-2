#include <cstddef>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <optional>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h> // Pour Windows
#else
#include <cstdlib>   // Pour Linux/macOS
#endif

void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    // Assume que les autres systèmes d'exploitation sont compatibles avec ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
#endif
}

/*
 * Représente un coordonnées dans le plateau
 */
class BoardCoordinates {
protected:
  size_t _x;
  size_t _y;

public:
  // Default constructor
  BoardCoordinates() : _x{0}, _y{0} {}

  BoardCoordinates(size_t x, size_t y) : _x{x}, _y{y} {}

  // Destructor
  virtual ~BoardCoordinates() = default;

  [[nodiscard]] constexpr inline size_t x() const { return _x; }
  [[nodiscard]] constexpr inline size_t y() const { return _y; }

  void set(size_t x, size_t y) {
    _x = x;
    _y = y;
  }
  // Supercharge the == operator

  bool operator==(const BoardCoordinates &other) const {
    return x() == other.x() && y() == other.y();
  }

  /** Whether c is in [A-Za-z] */
  constexpr static bool isalpha(char c) {
    // Contrary to std::isalpha, works with char and not modified by locale
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
  }

  /** "A" -> 0, "B" -> 1, ..., "AA" -> 26, ...
   * Any non alpha character (as defined by BoardCoordinates::isalpha) produces
   * a null result. */
  [[nodiscard]] static std::optional<size_t> parseX(const std::string &x_string) {
    if (x_string.empty()) {
      return std::nullopt;
    }

    size_t result{0};
    for (char c : x_string) {
      if (!isalpha(c)) {
        return std::nullopt;
      }
      result = result * 26 + static_cast<unsigned>(std::toupper(c) - 'A' + 1);
    }
    return result - 1;
  }

  /** "1" -> 0, "2" -> 1, ...
   * Uses std::stoull. Return null if std::stoull throws an exception. */
  [[nodiscard]] static std::optional<size_t> parseY(const std::string &y_string) {
    const int BASE{10};
    try {
      unsigned long long parsed = std::stoull(y_string, nullptr, BASE);
      if (parsed == 0) {
        return std::nullopt;
      }
      return parsed - 1;
    } catch (const std::logic_error &) {
      return std::nullopt;
    }
  }

  /** {0, 0} returns "A1" */
  [[nodiscard]] virtual inline std::string toString() const {
    return xToString() + yToString();
  }

  /** returns the x / letter part of toString() */
  [[nodiscard]] virtual inline std::string xToString() const {
    std::string result{};
    size_t n = _x + 1;
    while (n > 0) {
      result = static_cast<char>('A' + (n - 1) % 26) + result;
      n = (n - 1) / 26;
    }
    return result;
  }

  /** returns the y / number part of toString() */
  [[nodiscard]] virtual inline std::string yToString() const {
    return std::to_string(_y + 1);
  }
};

/** Put bc.toString() on os */
std::ostream &operator<<(std::ostream &os, const BoardCoordinates &bc);

/** Extract bc from os */
std::istream &operator>>(std::istream &is, BoardCoordinates &bc);

std::ostream& operator<<(std::ostream& os, const BoardCoordinates& bc) {
  os << bc.toString();
  return os;
}

std::istream& operator>>(std::istream& is, BoardCoordinates& bc) {
  std::string x_string;
  while (is.good() && !is.eof() &&
         BoardCoordinates::isalpha(static_cast<char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    x_string += c;
  }

  std::string y_string;
  while (is.good() && !is.eof() && std::isdigit(static_cast<unsigned char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    y_string += c;
  }

  std::optional<size_t> x{BoardCoordinates::parseX(x_string)};
  std::optional<size_t> y{BoardCoordinates::parseY(y_string)};

  if (x && y) {
    bc.set(x.value(), y.value());
  } else {
    is.setstate(std::ios_base::failbit);
  }

  return is;
}





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
  UNDAMAGED_SHIP = IS_SHIP,                 //< UNDAMAGED_SHIP ship, used for my side
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
case mine touché          ¤ - case mine touché      * - is_mine | is_kwown | is_hit

None                        - case bateau           █ - is_ship
case bateau scanné        █ - case bateau scanné    █ - is_ship | is_kwown
case bateau touché        ▒ - case bateau touché    ▒ - is_ship | is_kwown | is_hit
case bateau coulé         ░ - case bateau coulé     ░ - is_ship | is_kwown | is_hit | is_sunk
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
  UNDAMAGED_MINE = IS_MINE,
  SCANNED_MINE = IS_MINE | IS_KNOWN,
  HIT_MINE = IS_MINE | IS_KNOWN | IS_HIT,

  // Ship states:
  UNDAMAGED_SHIP = IS_SHIP,
  SCANNED_SHIP = IS_SHIP | IS_KNOWN,
  HIT_SHIP = IS_SHIP | IS_KNOWN | IS_HIT,
  SUNK_SHIP = IS_SHIP | IS_KNOWN | IS_HIT | IS_SUNK,
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
        case UNDAMAGED_MINE:
            return "¤ ";
        case HIT_SHIP:
            return "▒ ";
        case SUNK_SHIP:
            return "░ ";
    }
  }
};




class Ship {
  private:
    std::vector<BoardCoordinates> _coordinates;
    BoardCoordinates _top_left;
    int _size_x = 0;
    int _size_y = 0;
    int _number_of_case = 0;
    CellType _type = UNDAMAGED_SHIP;

  public:
    Ship(std::vector<BoardCoordinates> coordinates);
    void rotate();
    void print();
    std::vector<BoardCoordinates> getCoordinates();
    BoardCoordinates getTopLeft();
    int getNumberOfCase();
    CellType getType();
    void setTopLeft(BoardCoordinates top_left);
    void setType(CellType new_type);
};

Ship::Ship(std::vector<BoardCoordinates> coordinates): _coordinates(coordinates) {
    for (auto &c: coordinates) {
        _number_of_case++;
        if (c.x() + 1 > _size_x) { _size_x = c.x() + 1; }
        if (c.y() + 1 > _size_y) { _size_y = c.y() + 1; }
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
        to_print.at(c.y()).at(c.x()) = "██";
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

BoardCoordinates Ship::getTopLeft() {
    return _top_left;
}

CellType Ship::getType() {
    return _type;
}

int Ship::getNumberOfCase() {
    return _number_of_case;
}

void Ship::setTopLeft(BoardCoordinates top_left) {
    _top_left = top_left;
}

void Ship::setType(CellType new_type) {
    _type = new_type;
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
    void setTopLeft(BoardCoordinates top_left);
    void print();
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

void ShipClassic::setTopLeft(BoardCoordinates top_left) {
    _ships[_pos].setTopLeft(top_left);
}

void ShipClassic::print() {
    _ships[_pos].print();
}




class ShipCommander : public ShipClassic {
public:
    ShipCommander(int number_of_case);
};

ShipCommander::ShipCommander(int number_of_case): ShipClassic{number_of_case} {
    switch (number_of_case) {
      case 1:
        _ships.push_back(Ship({{0, 0}}));
        _ships[0].setType(UNDAMAGED_MINE);
        break;
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
  bool addShip(Ship s);

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

bool LocalBoard::addShip(Ship s) {
    BoardCoordinates top_left = s.getTopLeft();
    for (auto &c: s.getCoordinates()) {
        if (_my_board.at(top_left.y() + c.y()).at(top_left.x() + c.x()).type() != WATER) {
          return false;
        }
    }
    
    for (auto &c: s.getCoordinates()) {
        _my_board.at(top_left.y() + c.y()).at(top_left.x() + c.x()).setType(s.getType());
    }
    return true;
  }


/*
int main() {
	LocalBoard test;
    ShipClassic* ships = new ShipCommander(4);
    //ships->next();
    ships->next();
    //ships->next();
    //ships->next();
    //ships->next();
    //ships->next();
    ships->rotate();
    //ships->rotate();
    //ships->rotate();
    //ships->rotate();
    ships->print();
    //test.addShip(ships->getShip());
    //test.print();
    delete ships;
    
    //BoardCoordinates coordinates{};
    //std::cout << "\x1b[32;49;1m";
    //std::cin >> coordinates;
    //std::cout << "\x1b[0m";
    //std::cout << coordinates.x() << " " << coordinates.y() << std::endl;
    //std::cout << std::endl;
    return 0;
}
*/



int main() {
    LocalBoard test;
    std::array<uint8_t, 5> _ships_to_place = {1, 1, 2, 1, 1};

    bool boat_remaining = true;

    while (boat_remaining) {
        test.print();
        int i = 1;

        for (auto &b: _ships_to_place) {
        std::cout << "(" << i << ") - " << unsigned(b) << " boat left of size " << i << std::endl;
        i++;
        }

        bool invalid_input = true;
        bool input_error = false;
        std::string answer;
        int boat_selected;
        
        while (invalid_input) {            
            if (input_error) {
                std::cout << "Invalid Input !";
            }
            std::cout << std::endl;
            
            input_error = true;
            std::getline(std::cin, answer);
            if (!answer.empty()) {
                try {
                    boat_selected = stoi(answer);
                
                    if ((boat_selected > 0) and (boat_selected < 6)) {
                        if (_ships_to_place[boat_selected] != 0) {
                            invalid_input = false;
                        }
                    }
                }
                catch (const std::exception& e) {}
            }
        }
        ShipClassic* ships = new ShipCommander(boat_selected);
        
        invalid_input = true;
        input_error = false;

        while (invalid_input) {
            ships->print();
            std::cout << "(1) - next boat" << std::endl;
            std::cout << "(2) - rotate boat" << std::endl;
            std::cout << "(3) - select boat" << std::endl;

            if (input_error) {
                std::cout << "Invalid Input !";
            }
            std::cout << std::endl;
            
            input_error = true;
            std::getline(std::cin, answer);
            if (!answer.empty()) {
                try {
                    int value = stoi(answer);
                    switch (value) {
                      case 1:
                        input_error = false;
                        ships->next();
                        break;
                      case 2:
                        input_error = false;
                        ships->rotate();
                        break;
                      case 3:
                        invalid_input = false;
                      default:
                        break;
                    }
                }
                catch (const std::exception& e) {}
            }
        }
        std::cout << "select a position" << std::endl;
        BoardCoordinates coordinates{};
        std::cin >> coordinates;
        ships->setTopLeft(coordinates);
        test.addShip(ships->getShip());

        std::cout << boat_selected - 1 << std::endl;
        _ships_to_place[boat_selected - 1]--;
        
        boat_remaining = false;
        for (auto &b: _ships_to_place) {
            if (b != 0) {
                boat_remaining = true;
                break;
            }
        } 

        delete ships;
    }
}