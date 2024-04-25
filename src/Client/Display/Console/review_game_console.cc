#include "review_game_console.hh"
#include "review_game_controller.hh"
#include "local_board_commander.hh"
#include "not_implemented_error.hh"
#include "cell_type.hh"
#include <bitset>

using std::string;


ReviewGameConsole::ReviewGameConsole(std::ostream &out, std::istream &in,
                            std::shared_ptr<LocalBoardReview> board,
                            std::shared_ptr<ReviewGameController> controller)
    : _out{out}, _in{in}, _board{std::move(board)}, _controller{std::move(controller)},
    _print_info{
        static_cast<uint8_t>(length(BoardCoordinates(_board->width() - 1, _board->height() - 1).xToString())),
        static_cast<uint8_t>(length(BoardCoordinates(_board->width() - 1, _board->height() - 1).yToString())),
        "   ", 0, 0, createMapKey()}
    {
        std::clog << "REVIEWGAMECONSOLE: constructor" << std::endl;
        _print_info.grid_width = _print_info.number_width + 1 + (1 + _print_info.letter_width) * _board->width() + 1;
        _print_info.width = _print_info.grid_width * 2 + _print_info.gap.size();
    }


inline string operator*(const string &lhs, size_t rhs) {
    std::clog << "REVIEWGAMECONSOLE: operator." << std::endl;
    string result;
    result.reserve(lhs.size() * rhs);
    for (size_t i = 0; i < rhs; ++i) {
        result += lhs;
    }
    return result;
}


ReturnInput ReviewGameConsole::handleInput(){
    std::clog << "REVIEWGAMECONSOLE: handle input." << std::endl;
    std::string input;
    int number;
    while (true) {
        std::getline(std::cin, input);
        if (input == "./exit") {
            std::cout << "Exiting review...\n";
            return {ReturnInput::MAIN_MENU, ""};
        }
        else if (input == "N") {
            std::cout << "Next move...\n";
            _controller->setNextMove();
            display();
        } 
        else if (input == "P") {
            std::cout << "Previous move...\n";
            _controller->setPreviousMove();
            display();
        }
    }
}


void ReviewGameConsole::display(){
    std::clog << "REVIEWGAMECONSOLE: display." << std::endl;
    system("clear");
    displayHeader();
    displayGameReview();
    displayMapKey();
    displayActions();
}

/***************
**   Utils    **
****************/

std::vector<string> ReviewGameConsole::createMapKey() const {
    std::clog << "REVIEWGAMECONSOLE: create map key." << std::endl;
    std::vector<string> map_key;
    map_key.emplace_back(" > " + toString(OCEAN) + " Ocean          <");
    map_key.emplace_back(" > " + toString(UNDAMAGED_SHIP) + " UNDAMAGED ship <");
    map_key.emplace_back(" > " + toString(UNDAMAGED_MINE) + " UNDAMAGED mine <");
    map_key.emplace_back(" > " + toString(SCANNED_SHIP) + " SCANNED ship   <");
    map_key.emplace_back(" > " + toString(SCANNED_MINE) + " SCANNED mine   <");
    map_key.emplace_back(" > " + toString(HIT_SHIP) + " Hit ship       <");
    map_key.emplace_back(" > " + toString(HIT_MINE) + " Hit mine       <");
    map_key.emplace_back(" > " + toString(SUNK_SHIP) + " Sunk ship      <");
    return map_key;
}


string ReviewGameConsole::toString(CellType type) {
    std::bitset<8> x(static_cast<unsigned char>(type));
    std::clog << x << std::endl;
    std::clog << "REVIEWGAMECONSOLE: to string." << std::endl;
    switch (type) {
    case WATER:
      return " ";
    case OCEAN:
      return "╳";
    case SCANNED_SHIP:
    case UNDAMAGED_SHIP:
      return "█";
    case HIT_SHIP:
      return "▒";
    case SUNK_SHIP:
      return "░";
    case UNDAMAGED_MINE:
    case SCANNED_MINE:
      return "¤";
    case HIT_MINE:
      return "⚑";
    default:
      throw NotImplementedError("ConsoleBoardDisplay unknown CellType");
    }
}


constexpr size_t ReviewGameConsole::length(const string &s) {
    // In UTF-8, continuation bytes begin with 0b10, so, does not count these bytes.
    return static_cast<size_t>(std::ranges::count_if(
      s, [](char c) noexcept { return (c & '\xC0') != '\x80'; }));
}


/***************
**Print Methods**
****************/


void ReviewGameConsole::displayHeader(){
    std::clog << "REVIEWGAMECONSOLE: displayHeader." << std::endl;
    std::cout << "┌────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Review game: " << _board->getSessionId() << "\n";
    std::cout << "└────────────────────────────────────────────────────────────────────────────────┘\n";
}


void ReviewGameConsole::displayGameReview(){
    std::clog << "REVIEWGAMECONSOLE: displayGameReview." << std::endl;
    printSideBySide({createGrid(true)}, {createGrid(false)});
}

void ReviewGameConsole::displayMapKey(){
    std::cout << std::endl;
    std::clog << "REVIEWGAMECONSOLE: displayMapKey." << std::endl;
    for(auto line: _print_info.map_key){
        std::cout << line << std::endl;
    }
}


void ReviewGameConsole::displayActions(){
    std::cout << "┌────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│        Previous        ││          Exit            ││           Next           │\n";
    std::cout << "└────────────────────────────────────────────────────────────────────────────────┘\n";
    std::cout << "Type P for PREVIOUS, N for NEXT or './exit' to quit: \n";
}


void ReviewGameConsole::printSideBySide(std::vector<string> left,
                                  std::vector<string> right) {
    std::clog << "REVIEWGAMECONSOLE: printSideBySide." << std::endl;
    size_t left_width = std::max(_print_info.grid_width,
                       std::ranges::max(left, {}, [](const string &s) noexcept {return length(s);}).size());
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
        } else {_out << space;}
        // Right (and gap)
        if (idx < right.size()) {
            _out << _print_info.gap << right.at(idx);
        }
        // New line
        if (idx < last_line - 1) {_out << '\n';}
    }
}


/*******************
**Pre_print Methods**
********************/


std::vector<string> ReviewGameConsole::createGrid(bool my_side) {
    std::clog << "REVIEWGAMECONSOLE: createGrid." << std::endl;
    std::vector<string> grid;
    std::ostringstream oss("    ", std::ios_base::ate); 
    createLetters(oss);
    grid.emplace_back(oss.str());   
    createFirstLine(oss);
    grid.emplace_back(oss.str());   
    for (unsigned i = 0; i < _board->height(); ++i) {
      createBody(oss, i, my_side);
      grid.emplace_back(oss.str());
    }
    createLastLine(oss);
    grid.emplace_back(oss.str());   

    return grid;
}


void ReviewGameConsole::createLetters(std::ostringstream& oss){
    std::clog << "REVIEWGAMECONSOLE: createLetters." << std::endl;
    for (size_t i = 0; i < _board->width(); ++i) {
    oss << std::setw(_print_info.letter_width) << BoardCoordinates{i, 0}.xToString()
        << ' ';
    } 
}


void ReviewGameConsole::createFirstLine(std::ostringstream& oss){
    std::clog << "REVIEWGAMECONSOLE: createFirstLine." << std::endl;
    oss.str("   ┌");
    oss << (((string("─") * _print_info.letter_width) + "┬") * (_board->width() - 1));
    oss << "─┐";
}


void ReviewGameConsole::createBody(std::ostringstream& oss, int i, bool my_side){
    std::clog << "REVIEWGAMECONSOLE: createBody." << std::endl;
    oss.str("");
    oss << std::setw(_print_info.number_width) << i + 1 << " ";
    for (unsigned j = 0; j < _board->width(); ++j) {
        string border = "│";
        CellType content = _board->cellType(my_side, {j, i});
        if (j > 0 && _board->isSameShip(my_side, {j - 1, i}, {j, i})) {
            CellType previous = _board->cellType(my_side, {j - 1, i});
            if (previous == WATER) {
                border = "│";
            } else {
                border = toString(_board->best(content, previous));
            }
        }
        oss << border << toString(content);
    }
    oss << "│";
}


void ReviewGameConsole::createLastLine(std::ostringstream& oss){
    std::clog << "REVIEWGAMECONSOLE: createLastLine." << std::endl;
    oss.str("   └");
    oss << (((string("─") * _print_info.letter_width) + "┴") * (_board->width() - 1));
    oss << "─┘";
}
