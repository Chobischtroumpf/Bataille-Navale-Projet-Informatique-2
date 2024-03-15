#include "game_console.hh"
#include "local_board_commander.hh"

using std::string;

GameConsole::GameConsole(std::ostream& out, std::istream& in,
                         std::shared_ptr<LocalBoard> board,
                         std::shared_ptr<GameController> control,
                         std::shared_ptr<GameClient> client)
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
      _map_key{createMapKey()},
      _game_client(client) {}


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

    if (_board->myTurn() && !my_side && content == UNDAMAGED_SHIP){
        oss << border << toString(WATER);
      }else if (!_board->myTurn() && my_side && content == UNDAMAGED_SHIP){
        oss << border << toString(WATER);
      }else{
        if (j > 0 && _board->isSameShip(my_side, {j - 1, i}, {j, i})) {
          CellType previous = _board->cellType(my_side, {j - 1, i});
          
          if ((previous == UNDAMAGED_SHIP) && ((_board->myTurn() && !my_side) || (!_board->myTurn() && my_side))) {
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
  map_key.emplace_back(" > " + toString(UNDAMAGED_SHIP) + " UNDAMAGED ship <");
  map_key.emplace_back(" > " + toString(UNDAMAGED_MINE) + " UNDAMAGED mine <");
  map_key.emplace_back(" > " + toString(SCANNED_SHIP) + " SCANNED ship <");
  map_key.emplace_back(" > " + toString(SCANNED_MINE) + " SCANNED mine <");
  map_key.emplace_back(" > " + toString(HIT_SHIP) + " Hit ship       <");
  map_key.emplace_back(" > " + toString(HIT_MINE) + " Hit mine       <");
  map_key.emplace_back(" > " + toString(SUNK_SHIP) + " Sunk ship      <");
  return map_key;
}

std::vector<string> GameConsole::createBoatsKey() const {
    std::vector<string> boat_key;
    std::vector<PossibleShip> remaining_ships = _board->shipsToPlace();
    std::array<std::string, 3> color_code = {"\x1B[2m", "\x1B[0m", "\x1B[0m"};
    //std::cout << shipCounts[CARRIER] <<" " << shipCounts[BATTLESHIP] << " " << std::endl;
    boat_key.emplace_back("");
    for (auto &ship : remaining_ships) {
      switch (ship.second) {
        case 1:
          boat_key.emplace_back(color_code.at(ship.first) + " > " + toString(UNDAMAGED_MINE) * 1 + "          Mine     (×"+ std::to_string(ship.first) +") <" + color_code.at(1));
          break;
        case 2:
          boat_key.emplace_back(color_code.at(ship.first) + " > " + toString(UNDAMAGED_SHIP) * 3 + "        Destroyer  (×" + std::to_string(ship.first) + ") <" + color_code.at(1));
          break;
        case 3:
          boat_key.emplace_back(color_code.at(ship.first) + " > " + toString(UNDAMAGED_SHIP) * 5 + "      Submarine  (×"+ std::to_string(ship.first) +") <" + color_code.at(1));
          break;
        case 4:
          boat_key.emplace_back(color_code.at(ship.first) + " > " + toString(UNDAMAGED_SHIP) * 7 + "    Battleship (×"+ std::to_string(ship.first) +") <" + color_code.at(1));
          break;
        case 5:
          boat_key.emplace_back(color_code.at(ship.first) + " > " + toString(UNDAMAGED_SHIP) * 9 + "  Carrier    (×"+ std::to_string(ship.first) +") <" + color_code.at(1));
          break;
        default:
          break;
      }
    }
    return boat_key;
  }

std::vector<string> GameConsole::createPlaceShipPrompt(InputStatus status) const { return {}; } // WARNING: TODO

std::vector<string> GameConsole::createGamePrompt(InputStatus status) const {
  std::vector<string> prompt(_map_key.size() - 2, "");  // Add padding
  prompt.emplace_back(">> SELECT TARGET <<");
  prompt.emplace_back(">> ");
  return prompt;
}

std::vector<string> GameConsole::createSelectShipSizePrompt(InputStatus status) const {
  std::vector<string> prompt(_map_key.size()-4, "");  // Add padding
  prompt.emplace_back("");
  prompt.emplace_back("Select the size of the boat you want to place");
  if (status == OK) {
    prompt.emplace_back("");
  } else {
    prompt.emplace_back("\x1B[31m Invalid input, please try again. \x1B[0m");
  }
  prompt.emplace_back(">> BOAT SIZE <<");
  prompt.emplace_back(">> ");
  return prompt;
}

std::vector<string> GameConsole::createSelectNextRotateKey(InputStatus status) const {
  std::vector<string> action_key(_map_key.size()-6, "");
  action_key.emplace_back("");
  action_key.emplace_back("> 1 - Next boat shape <");
  action_key.emplace_back("> 2 - Rotate boat     <");
  action_key.emplace_back("> 3 - Select boat     <");
  if (status == OK) {
    action_key.emplace_back("");
  } else {
    action_key.emplace_back("\x1B[31m Invalid input, please try again. \x1B[0m");
  }
  action_key.emplace_back(">> SELECT ACTION <<");
  action_key.emplace_back(">> ");
  return action_key;
}

std::vector<string> GameConsole::createSelectShipPositionPrompt(InputStatus status) const {
  std::vector<string> prompt(_map_key.size()-4, "");  // Add padding
  prompt.emplace_back("");
  prompt.emplace_back("Select the position where you want to place de boat (the position selected is the top left corner of the boat)");
  if (status == OK) {
    prompt.emplace_back("");
  } else {
    prompt.emplace_back("\x1B[31m Invalid input, please try again. \x1B[0m");
  }
  prompt.emplace_back(">> PLACE SHIP <<");
  prompt.emplace_back(">> ");
  return prompt;
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
        BoardCoordinates coordinates{};
        _out << "\x1b[32;49;1m";
        _in >> coordinates;
        _out << "\x1b[0m";

        if (std::cin.eof()) {
            _out << std::endl;
            _control->quit();
            return;
        }
        if (_in && coordinates.x() < _board->width() &&
              coordinates.y() < _board->height()) {
          // _control->placeShip(coordinates);
          _valid_last_input = true;
        } else {
          _valid_last_input = false;
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
updatePlaceShip(_valid_last_input ? OK : ERR);
}
void GameConsole::displayError() {}
void GameConsole::update() {}
ReturnInput GameConsole::handleInput() {
  handlePlaceShip();
}

constexpr size_t GameConsole::length(const string& s) {
    // In UTF-8, continuation bytes begin with 0b10, so, does not count these bytes.
    return static_cast<size_t>(
        std::ranges::count_if(s, [](char c) noexcept { return (c & '\xC0') != '\x80'; }));
}