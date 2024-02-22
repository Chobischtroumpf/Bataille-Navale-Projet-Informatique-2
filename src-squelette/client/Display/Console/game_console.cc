#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

#include "game_console.hh"
#include "../../local_board.hh"

using std::string;

GameConsole::GameConsole(std::ostream& out, std::istream& in,
                         std::shared_ptr<LocalBoard> board,
                         std::shared_ptr<GameController> control)
    : _out{out},
      _in{in},
      _board{std::move(board)},
      _control{std::move(control)},
      _letter_width{static_cast<uint8_t>(
              length(BoardCoordinates(_board->width() - 1, _board->height() - 1)
                             .xToString()))},
      _number_width{static_cast<uint8_t>(
              length(BoardCoordinates(_board->width() - 1, _board->height() - 1)
                             .yToString()))},
      _gap{"   "},
      _grid_width{_number_width + 1 +
                  (1 + _letter_width) * _board->width() + 1},
      _width{_grid_width * 2 + _gap.size()},
      _map_key{createMapKey()} {}

inline string operator*(const string& lhs, size_t rhs) {
  string result;
  result.reserve(lhs.size() * rhs);
  for (size_t i = 0; i < rhs; ++i) {
    result += lhs;
  }
  return result;
}

string GameConsole::createGameHeader() const {
  //                   ╔════════════╗
  //                   ║ Your Turn  ║
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

string GameConsole::createPlaceShipHeader() const {
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

string GameConsole::createGridLabel(bool my_side) const {
  string your        = "Player 1";
  string their       = "Player 2";
  size_t label_size  = std::max(length(your), length(their));
  size_t margin_size = label_size > _grid_width ? 0 : (_grid_width - label_size) / 2;
  string margin(margin_size, ' ');
  return margin + (my_side ? your : their);
}

std::vector<string> GameConsole::createGrid(bool my_side) const {
  std::vector<string>     grid;
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

std::vector<string> GameConsole::createMapKey() const {
  std::vector<string> map_key;
  map_key.emplace_back(" > " + toString(OCEAN) + " Ocean          <");
  map_key.emplace_back(" > " + toString(UNDAMAGED) + " Undamaged ship <");
  map_key.emplace_back(" > " + toString(HIT) + " Hit ship       <");
  map_key.emplace_back(" > " + toString(SUNK) + " Sunk ship      <");
  return map_key;
}

std::vector<string> GameConsole::createBoatsKey() const {
    std::vector<string> boat_key;
    std::array<std::pair<ShipType, uint8_t>, 4> remaining_ships = _board->shipsToPlace();
    uint8_t remaining_destroyer = remaining_ships.at(DESTROYER-2).second;
    uint8_t remaining_submarine = remaining_ships.at(SUBMARINE-2).second;
    uint8_t remaining_battleship = remaining_ships.at(BATTLESHIP-2).second;
    uint8_t remaining_carrier = remaining_ships.at(CARRIER-2).second;
    std::array<std::string, 3> color_code = {"\x1B[2m", "\x1B[0m", "\x1B[0m"};
    //std::cout << shipCounts[CARRIER] <<" " << shipCounts[BATTLESHIP] << " " << std::endl;
    boat_key.emplace_back("");
    boat_key.emplace_back(color_code.at(remaining_destroyer) + " > " + toString(UNDAMAGED) * 3 + "        Destroyer  (×" + std::to_string(remaining_destroyer) + ") <" + color_code.at(1));
    boat_key.emplace_back(color_code.at(remaining_submarine) + " > " + toString(UNDAMAGED) * 5 + "      Submarine  (×"+ std::to_string(remaining_submarine) +") <" + color_code.at(1));
    boat_key.emplace_back(color_code.at(remaining_battleship) + " > " + toString(UNDAMAGED) * 7 + "    Battleship (×"+ std::to_string(remaining_battleship) +") <" + color_code.at(1));
    boat_key.emplace_back(color_code.at(remaining_carrier) + " > " + toString(UNDAMAGED) * 9 + "  Carrier    (×"+ std::to_string(remaining_carrier) +") <" + color_code.at(1));
    return boat_key;
  }

std::vector<string> GameConsole::createGamePrompt(InputStatus status) const {
  std::vector<string> prompt(_map_key.size() - 2, "");  // Add padding
  prompt.emplace_back(">> SELECT TARGET <<");
  prompt.emplace_back(">> ");
  return prompt;
}

std::vector<string> GameConsole::createPlaceShipPrompt(InputStatus status) const {
  std::vector<string> prompt(_map_key.size()-4, "");  // Add padding
  prompt.emplace_back("");
  prompt.emplace_back("Enter the Ship ID, the H or V for horizontal or vertical, then X and Y coordinates (e.g. 4 V C4):");
  if (status == OK) {
    prompt.emplace_back("");
  } else {
    prompt.emplace_back("\x1B[31m Invalid input, please try again. \x1B[0m");
  }
  prompt.emplace_back(">> PLACE SHIP <<");
  prompt.emplace_back(">> ");
  return prompt;
}

void GameConsole::print(const std::vector<string>& lines) {
  for (const string& line : lines) {
    _out << line << '\n';
  }
}

void GameConsole::clearBadGameInput(bool placed) {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (placed) {
    updateGame(OK);
  } else {
    updateGame(ERR);
  }
}

void GameConsole::clearBadPlaceShipInput(bool placed) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (placed) {
      if (_board->allBoatsPlaced()) {
        _control->sendShips(_board->getPlacedShips());
        waitGame();
      } else {
        updatePlaceShip(OK);
      }
    } else {
      updatePlaceShip(ERR);
    }
}

void GameConsole::printSideBySide(std::vector<string> left, std::vector<string> right) {
  size_t left_width = std::max(
      _grid_width,
      std::ranges::max(left, {}, [](const string& s) noexcept { return length(s); }).size());
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


void GameConsole::printChangeTurn() {
  string your        = "Player 1";
  string their       = "Player 2";
  string tmp;

  std::system("clear");
  
  string who = _board->myTurn() ? your : their;
  _out << who << std::endl << "Please press enter to continue..." << std::flush;
  std::cerr << "printChangeTurn" << std::endl;
  // std::getline(std::cin, tmp);
  //std::cin >> tmp;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void GameConsole::handleFire() {
  if (_board->myTurn()) {
    for (bool fired = false; !fired; clearBadGameInput(fired)) {
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


void GameConsole::handlePlaceShip() {
  if (_board->myTurn()) {
    for (bool placed = true; !_board->allBoatsPlaced(); clearBadPlaceShipInput(placed)) {
        ShipCoordinates coordinates{};
        _out << "\x1b[32;49;1m";
        _in >> coordinates;
        _out << "\x1b[0m";

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

void GameConsole::updateGame(InputStatus status) {
  //methode d'affichage d'ecran temporaire pour le changement de tour
  std::system("clear");  // Do not use std::system in other contexts
  _out << createGameHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->myTurn()) {
    printSideBySide(createMapKey(), createGamePrompt(status));
    handleFire();
  } else {
    print(createMapKey());
    _board->waitGame(); // Wait for the other player to play
  }
  _out << std::flush;
}

void GameConsole::updatePlaceShip(InputStatus status) {
  //methode d'affichage d'ecran temporaire pour le changement de tour
  std::system("clear");  // Do not use std::system in other contexts
  _out << createGameHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  if (_board->myTurn()) {
    printSideBySide(createBoatsKey(), createPlaceShipPrompt(status));
    handlePlaceShip();
  } else {
    print(createBoatsKey());
  }
  _out << std::flush;
}

void GameConsole::waitGame() {
  std::system("clear");
  _out << createGameHeader();
  printSideBySide({createGridLabel(true)}, {createGridLabel(false)});
  _out << '\n';
  printSideBySide(createGrid(true), createGrid(false));
  _out << '\n';
  _out << "\x1b[34;1m Waiting for the other player to place their boats... \x1b[0m";
  _out << std::flush;
  _board->waitGame();
}

void GameConsole::display() {
  updatePlaceShip(OK);
}
void GameConsole::display_error() {}
void GameConsole::update() {}
ReturnInput GameConsole::handle_input() {
  handlePlaceShip();
}

constexpr size_t GameConsole::length(const string& s) {
    // In UTF-8, continuation bytes begin with 0b10, so, does not count these bytes.
    return static_cast<size_t>(
        std::ranges::count_if(s, [](char c) noexcept { return (c & '\xC0') != '\x80'; }));
}