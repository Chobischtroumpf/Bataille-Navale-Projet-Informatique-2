#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

#include "console_board_gen_display.hh"
#include "not_implemented_error.hh"

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

string ConsoleBoardGenDisplay::createHeader() const {
  //                   ╔═══════════════╗
  //                   ║ Ship to place ║
  //                   ╚═══════════════╝
  
  string ship;
  switch (_board->nbrBoats()){
      case 0:
         ship = repeat(2);
         break;
      case 1:
      case 2:
          ship = repeat(3);
          break;
      case 3:
          ship = repeat(4);
          break;
      case 4:
          ship = repeat(5);
          break;
      default:
         break; 
  }
  
  // 2de line:
  string ship_to_place = "║ Ship to place : " + ship + " ║";

  // margin:
  size_t margin_size = length(ship_to_place) > _width ? 0 : (_width - length(ship_to_place)) / 2;
  string margin(margin_size, ' ');

  // 1st and 3rd line:
  std::ostringstream oss;
  oss << (string("═") * (length(ship_to_place) - 2));
  string line = oss.str();
  oss.str("");  // clear oss

  // Result:
  oss << margin << "╔" << line << "╗\n"
      << margin << ship_to_place << '\n'
      << margin << "╚" << line << "╝\n\n";
  return oss.str();
}

string ConsoleBoardGenDisplay::createGridLabel(bool my_side) const {
  string your        = "Player 1";
  string their       = "Player 2";
  size_t label_size  = std::max(length(your), length(their));
  size_t margin_size = label_size > _grid_width ? 0 : (_grid_width - label_size) / 2;
  string margin(margin_size, ' ');
  return margin + (my_side ? your : their);
}

vector<string> ConsoleBoardGenDisplay::createGrid(bool my_side) const {
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
// check is my_side == false et content == BoardView::UNDAMAGED alors on affiche BoardView::WATER
       
      
    if (_board->myTurn() && !my_side && content == BoardView::UNDAMAGED){
        oss << border << toString(BoardView::WATER);
      }else if (!_board->myTurn() && my_side && content == BoardView::UNDAMAGED){
        oss << border << toString(BoardView::WATER);
      }else{
        if (j > 0 && _board->isSameShip(my_side, {j - 1, i}, {j, i})) {
          BoardView::CellType previous = _board->cellType(my_side, {j - 1, i});
          
          if ((previous == BoardView::UNDAMAGED) && ((_board->myTurn() && !my_side) || (!_board->myTurn() && my_side))) {
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

vector<string> ConsoleBoardGenDisplay::createPrompt() const {
  vector<string> prompt(_map_key.size() - 2, "");  // Add padding
  prompt.emplace_back(">> SELECT TARGET <<");
  prompt.emplace_back(">> ");
  return prompt;
}

void ConsoleBoardGenDisplay::printSideBySide(vector<string> left, vector<string> right) {
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

void ConsoleBoardGenDisplay::print(const vector<string>& lines) {
  for (const string& line : lines) {
    _out << line << '\n';
  }
}

void ConsoleBoardGenDisplay::clearBadInput() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  update();
}

void ConsoleBoardGenDisplay::printChangeTurn() {
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

void ConsoleBoardGenDisplay::handleInput() {

  for (bool fired = false; !fired; clearBadInput()) {
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

void ConsoleBoardGenDisplay::update() {
  //methode d'affichage d'ecran temporaire pour le changement de tour
  std::system("clear");  // Do not use std::system in other contexts
  _out << createHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->myTurn()) {
    printSideBySide(createBoatsKey(), createPrompt());
  } else {
    printSideBySide(createBoatsKey(), createPrompt());
    // print(createMapKey());
  }
  _out << std::flush;
}

