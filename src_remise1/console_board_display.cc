#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

#include "console_board_display.hh"
#include "not_implemented_error.hh"
#include "ship_coordinates.hh"

namespace ranges = std::ranges;
using std::string;

inline string operator*(const string& lhs, size_t rhs) {
  string result;
  result.reserve(lhs.size() * rhs);
  for (size_t i = 0; i < rhs; ++i) {
    result += lhs;
  }
  return result;
}

string ConsoleBoardDisplay::createGameHeader() const {
  //                   ╔════════════╗
  //                   ║ Your  Turn ║
  //                   ╚════════════╝

  // 2de line:
  string who  = _board->myTurn() ? "Player 1's " : "Player 2's";
  string turn = "║ " + who + " Turn ║";

  // margin:
  size_t margin_size = length(turn) > _width ? 0 : (_width - length(turn)) / 2;
  string margin(margin_size, ' ');

  // 1st and 3rd line:
  std::ostringstream oss;
  oss << (string("═") * (length(turn) - 2));
  string line = oss.str();
  oss.str("");  // clear oss

  // Result:
  oss << margin << "╔" << line << "╗\n"
      << margin << turn << '\n'
      << margin << "╚" << line << "╝\n\n";
  return oss.str();
}

string ConsoleBoardDisplay::createPlaceShipHeader() const {
  //                   ╔═══════════════╗
  //                   ║ Ship to place ║
  //                   ╚═══════════════╝

  // 2de line:
  string who = _board->myTurn() ? "Player 1:" : "Player 2:";
  string ship_to_place = "║ Place your ship ║";

  // margin:
  size_t margin = length(ship_to_place) > _width ? 0 : (_width - length(ship_to_place)) / 2;
  size_t margin_size_who  = ((length(ship_to_place)) - length(who)) > 0 ? (length(ship_to_place)-1 - length(who)) / 2 : 0;
  string margin_ship(margin, ' ');
  string margin_who(margin_size_who, ' ');

  // 1st and 3rd line:
  std::ostringstream oss;
  oss << (string("═") * (length(ship_to_place) - 2));
  string line = oss.str();
  oss.str("");  // clear oss

  // Result:
  oss << margin_ship << "╔" << line << "╗\n"
      << margin_ship << "║" << margin_who << who << margin_who << "║\n" 
      << margin_ship << ship_to_place << '\n'
      << margin_ship << "╚" << line << "╝\n\n";
  return oss.str();
}

string ConsoleBoardDisplay::createGridLabel(bool my_side) const {
  string your        = "Player 1";
  string their       = "Player 2";
  size_t label_size  = std::max(length(your), length(their));
  size_t margin_size = label_size > _grid_width ? 0 : (_grid_width - label_size) / 2;
  string margin(margin_size, ' ');
  return margin + (my_side ? your : their);
}

vector<string> ConsoleBoardDisplay::createGrid(bool my_side) const {
  vector<string>     grid;
  std::ostringstream oss("    ", std::ios_base::ate);

  // letters
  for (size_t i = 0; i < _board->width(); ++i) {
    oss << std::setw(_letter_width) << BoardCoordinates{i, 0}.xToString() << ' ';
  }
  grid.emplace_back(oss.str());

  // first line
  oss.str("   ┌");
  oss << (((string("─") * _letter_width) + "┬") * (_board->width() - 1));
  oss << "─┐";
  grid.emplace_back(oss.str());

  // body
  for (unsigned i = 0; i < _board->height(); ++i) {
    oss.str("");
    oss << std::setw(_number_width) << i + 1 << " ";
    for (unsigned j = 0; j < _board->width(); ++j) {
      string              border  = "│";
      CellType content = _board->cellType(my_side, {j, i});

    if (_board->myTurn() && !my_side && content == UNDAMAGED){
        oss << border << toString(WATER);
      }else if (!_board->myTurn() && my_side && content == UNDAMAGED){
        oss << border << toString(WATER);
      }else{
        if (j > 0 && _board->isSameShip(my_side, {j - 1, i}, {j, i})) {
          CellType previous = _board->cellType(my_side, {j - 1, i});
          
          if ((previous == UNDAMAGED) && ((_board->myTurn() && !my_side) || (!_board->myTurn() && my_side))) {
            border  = "│";
          }else{
            border = toString(_board->best(content, previous));
          }
        }
        oss << border << toString(content);
      }     
    }
    oss << "│";
    grid.emplace_back(oss.str());
  }

  // last line
  oss.str("   └");
  oss << (((string("─") * _letter_width) + "┴") * (_board->width() - 1));
  oss << "─┘";
  grid.emplace_back(oss.str());

  return grid;
}

vector<string> ConsoleBoardDisplay::createMapKey() const {
  vector<string> map_key;
  map_key.emplace_back(" > " + toString(OCEAN) + " Ocean          <");
  map_key.emplace_back(" > " + toString(UNDAMAGED) + " Undamaged ship <");
  map_key.emplace_back(" > " + toString(HIT) + " Hit ship       <");
  map_key.emplace_back(" > " + toString(SUNK) + " Sunk ship      <");
  return map_key;
}

vector<string> ConsoleBoardDisplay::createBoatsKey() const {
    vector<string> boat_key;
    std::map<ShipType, int> shipCounts = _board->countShips(_board->myTurn());
    //std::cout << shipCounts[CARRIER] <<" " << shipCounts[BATTLESHIP] << " " << std::endl;
    boat_key.emplace_back("");
    boat_key.emplace_back(" > id: 2 " + toString(UNDAMAGED) * 3 + "        Carrier (×"+std::to_string(1-shipCounts[CARRIER])+")    <");
    boat_key.emplace_back(" > id: 3 " + toString(UNDAMAGED) * 5 + "      Battleship (×"+std::to_string(2-shipCounts[BATTLESHIP])+") <");
    boat_key.emplace_back(" > id: 4 " + toString(UNDAMAGED) * 7 + "    Cruiser    (×"+std::to_string(1-shipCounts[CRUISER])+") <");
    boat_key.emplace_back(" > id: 5 " + toString(UNDAMAGED) * 9 + "  Submarine (×"+std::to_string(1-shipCounts[SUBMARINE])+")  <");
    return boat_key;
  }

vector<string> ConsoleBoardDisplay::createGamePrompt() const {
  vector<string> prompt(_map_key.size() - 2, "");  // Add padding
  prompt.emplace_back(">> SELECT TARGET <<");
  prompt.emplace_back(">> ");
  return prompt;
}

vector<string> ConsoleBoardDisplay::createPlaceShipPrompt() const {
  vector<string> prompt(_map_key.size()-4, "");  // Add padding
  prompt.emplace_back("");
  prompt.emplace_back("Enter the Ship ID, the H or V for horizontal or vertical, then X and Y coordinates (e.g. 4 V C4):");
  prompt.emplace_back("");
  prompt.emplace_back(">> PLACE SHIP <<");
  prompt.emplace_back(">> ");
  return prompt;
}


void ConsoleBoardDisplay::printSideBySide(vector<string> left, vector<string> right) {
  size_t left_width = std::max(
      _grid_width,
      ranges::max(left, {}, [](const string& s) noexcept { return length(s); }).size());
  size_t idx{0};
  size_t last_line = std::max(left.size(), right.size());
  string space(left_width, ' ');
  for (idx = 0; idx < last_line; ++idx) {
    // Left
    if (idx < left.size()) {
      _out << std::left << left.at(idx);
      if (length(left.at(idx)) < left_width) {
        _out << string(left_width - length(left.at(idx)), ' ');
      }
    } else {
      _out << space;
    }
    // Right (and gap)
    if (idx < right.size()) {
      _out << _gap << right.at(idx);
    }
    // New line
    if (idx < last_line - 1) {
      _out << '\n';
    }
  }
}

void ConsoleBoardDisplay::print(const vector<string>& lines) {
  for (const string& line : lines) {
    _out << line << '\n';
  }
}

void ConsoleBoardDisplay::clearBadGameInput() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  updateGame();
}

void ConsoleBoardDisplay::clearBadPlaceShipInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    updatePlaceShip();
}

void ConsoleBoardDisplay::printChangeTurn() {
  string your        = "Player 1";
  string their       = "Player 2";
  string tmp;

  std::system("clear");
  
  string who = _board->myTurn() ? your : their;
  std::cout << who << std::endl << "Please press enter to continue..." << std::flush;
  std::cerr << "printChangeTurn" << std::endl;
  // std::getline(std::cin, tmp);
  //std::cin >> tmp;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleBoardDisplay::handleFire() {
  if (_board->whoseTurn() == _turn) {
    for (bool fired = false; !fired; clearBadGameInput()) {
      BoardCoordinates coordinates{_board->width(), _board->height()};
      _in >> coordinates;

      if (std::cin.eof()) {
        _out << std::endl;
        _control->quit();
        return;
      }
      if (!(_in && coordinates.x() < _board->width() &&
                        coordinates.y() < _board->height())) {
        continue;
      }

      fired = _control->fire(coordinates);
    }
  }
}


void ConsoleBoardDisplay::handlePlaceShip() {
  if (_board->whoseTurn() == _turn) {
    for (bool placed = false; !placed; clearBadPlaceShipInput()) {
        ShipCoordinates coordinates{};
        _in >> coordinates;

        if (std::cin.eof()) {
            _out << std::endl;
            _control->quit();
            return;
        }
        if (!(_in && coordinates.x() < _board->width() &&
              coordinates.y() < _board->height())) {
            continue;
        }

        placed = _control->placeShip(coordinates);
    }
  }
}

void ConsoleBoardDisplay::  updateGame() {
  //methode d'affichage d'ecran temporaire pour le changement de tour
  std::system("clear");  // Do not use std::system in other contexts
  _out << createGameHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->whoseTurn() == _turn) {
    printSideBySide(createMapKey(), createGamePrompt());
  } else {
    print(createMapKey());
  }
  _out << std::flush;
}

void ConsoleBoardDisplay::updatePlaceShip() {
  //methode d'affichage d'ecran temporaire pour le changement de tour
  std::system("clear");  // Do not use std::system in other contexts
  _out << createPlaceShipHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->whoseTurn() == _turn) {
    printSideBySide(createBoatsKey(), createPlaceShipPrompt());
  } else {
    print(createBoatsKey());
  }
  _out << std::flush;
}

