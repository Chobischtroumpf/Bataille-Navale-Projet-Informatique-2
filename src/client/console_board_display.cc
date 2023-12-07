#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

#include "console_board_display.hh"
#include "not_implemented_error.hh"

namespace ranges = std::ranges;
using std::string;

string operator*(const string& lhs, size_t rhs) {
  string result;
  result.reserve(lhs.size() * rhs);
  for (size_t i = 0; i < rhs; ++i) {
    result += lhs;
  }
  return result;
}

string ConsoleBoardDisplay::createHeader() const {
  //                   ╔════════════╗
  //                   ║ Your  Turn ║
  //                   ╚════════════╝

  // 2de line:
  string who  = _board->myTurn() ? "Your " : "Their";
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

string ConsoleBoardDisplay::createGridLabel(bool my_side) const {
  string your        = "Your  Fleet";
  string their       = "Their Fleet";
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
      BoardView::CellType content = _board->cellType(my_side, {j, i});
      if (j > 0 && _board->isSameShip(my_side, {j - 1, i}, {j, i})) {
        BoardView::CellType previous = _board->cellType(my_side, {j - 1, i});
        border                       = toString(_board->best(content, previous));
      }
      oss << border << toString(content);
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

vector<string> ConsoleBoardDisplay::createPrompt() const {
  vector<string> prompt(_map_key.size() - 2, "");  // Add padding
  prompt.emplace_back(">> SELECT TARGET <<");
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

void ConsoleBoardDisplay::clearBadInput() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  update();
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


void ConsoleBoardDisplay::handleInput() {
  if (!_board->myTurn()) {
    throw NotImplementedError("handleInput() when not my turn.");
  }

  for (bool fired = false; !fired; clearBadInput()) {
    BoardCoordinates coordinates{_board->width(), _board->height()};
    std::cin >> coordinates;

    if (std::cin.eof()) {
      _out << std::endl;
      _control->quit();
      return;
    }
    if (!(std::cin && coordinates.x() < _board->width() &&
                      coordinates.y() < _board->height())) {
      continue;
    }

    ShipCoordinates coord;
    coord.anchor = BoardCoordinates(coordinates.x, coordinates.y);
    coord.vertical = true;
    coord.length = 3;
    fired = _control->placeShip();
  }
}

void ConsoleBoardDisplay::update() {
  std::system("clear");  // Do not use std::system in other contexts
  _out << createHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->myTurn()) {
    printSideBySide(createMapKey(), createPrompt());
  } else {
      printSideBySide(createMapKey(), createPrompt());
  }
  _out << std::flush;
}
