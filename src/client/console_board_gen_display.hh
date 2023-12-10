#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// #include "test.hh"
#include "board_control.hh"
#include "board_display.hh"
#include "board_view.hh"
#include "not_implemented_error.hh"

namespace ranges = std::ranges;
using std::vector;

/** BoardDisplay using text.
 *
 * A grid is a side of the board as represented on the screen.
 * Draw both sides of the board as two grids side by side. */
class ConsoleBoardGenDisplay final : public BoardDisplay {
  /** ConsoleBoardGenDisplay's methods must check they support this */
  std::ostream&                          _out;      //< Where to print
  std::istream&                          _in;       //< Where to read
  std::shared_ptr<BoardView const> const _board;    //< What to print
  std::shared_ptr<BoardControl> const    _control;  //< Who to inform of user actions

  uint8_t const _letter_width;  //< Number of character in a column name
  uint8_t const _number_width;  //< Number of character in a row name

  string const   _gap;         //< The string used as grid separator
  size_t const   _grid_width;  //< The number of character in a line of a grid
  size_t const   _width;    //< The number of character in a line with two grids and a gap
  vector<string> _map_key;  //< The map key, each string is a line without the ending '\n'

  // Utility methods

  /** Count number of unicode characters in a UTF-8 encoded string (assume linux platform)
   */
  static constexpr size_t length(const string& s) {
    // In UTF-8, continuation bytes begin with 0b10, so, does not count these bytes.
    return static_cast<size_t>(
        ranges::count_if(s, [](char c) noexcept { return (c & '\xC0') != '\x80'; }));
  }

  // Pre-print methods

  /** Single character to represent a type of cell on the board */
  static string toString(CellType type) {
    switch (type) {
      case WATER:
        return " ";
      case OCEAN:
        return "╳";
      case UNDAMAGED:
        return "█";
      case HIT:
        return "▒";
      case SUNK:
        return "░";
      default:
        throw NotImplementedError("ConsoleBoardGenDisplay unknown CellType");
    }
  }

  /** Create a header indicating who is setting the boats, and which boat they are setting */
  [[nodiscard]] string createHeader() const;

  /** Create a grid label: Player 1 / Player 2 Fleet */
  [[nodiscard]] string createGridLabel(bool my_side) const;

  /** Create grid for a given side, each string is a line without ending '\n' */
  [[nodiscard]] vector<string> createGrid(bool my_side) const;

  [[nodiscard]] static inline std::string repeat(int n) {
    std::ostringstream os;
    for(int i = 0; i < n; i++)
        os << toString(UNDAMAGED);
    return os.str();
  }

  /** Create map key, each string is a line without ending '\n'
   * Used by the constructor to init _map_key.
   * Outside constructor, use the attribute instead. */
  [[nodiscard]] vector<string> createBoatsKey() {
    vector<string> boat_key;
    boat_key.emplace_back("");
    boat_key.emplace_back(" > " + repeat(3) + (_board->nbrBoats()>0 ? " × 0": " × 1") + "        <" );
    boat_key.emplace_back(" > " + repeat(5) + (_board->nbrBoats()>1 ? (_board->nbrBoats()>2? " × 0": " × 1"): " × 2") + "      <" );
    boat_key.emplace_back(" > " + repeat(7) + (_board->nbrBoats()>3 ? " × 0": " × 1") + "    <" );
    boat_key.emplace_back(" > " + repeat(9) + (_board->nbrBoats()>4 ? " × 0": " × 1") + "  <" );
    return boat_key;
  }

  /** Create coordinates prompt, each string is a line without ending '\n'.
   * Empty lines are added in the beginning of the prompt so it can be printed next to the
   * map key. */
  [[nodiscard]] vector<string> createPrompt() const;

  // Print methods

  /** Print a vector of lines adding '\n' at the end of each line */
  void print(const vector<string>& lines);

  /** Print both parts side by side with _gap in between.
   * left and right are vector of lines without ending '\n'.
   * Both parts are fully printed, even if one part is shorter than the other.
   * If the left-hand part has lines of varying sizes, padding is added to ensure that the
   * right-hand part is properly aligned. Moreover, padding is also added so that the left
   * part is at least as wide as a grid.
   * The last line is printed without final '\n'. */
  void printSideBySide(vector<string> left, vector<string> right);

  // Input methods
  /** clear fail bits of _in, ignore until next '\n', redraw */
  void clearBadInput();

 public:
  ConsoleBoardGenDisplay() = delete;

  /** @param out: where to print
   * @param in: from where to read
   * @param board: what to print, must inherit from BoardView
   * @param control: who to inform of user actions.
   *
   * WARNING: ConsoleBoardGenDisplay does not support a change in board->height() or
   * board->width() after construction. */
  ConsoleBoardGenDisplay(std::ostream& out, std::istream& in,
                      std::shared_ptr<BoardView const> board,
                      std::shared_ptr<BoardControl>    control)
      : _out{out},
        _in{in},
        _board{std::move(board)},
        _control{std::move(control)},
        _letter_width{static_cast<uint8_t>(length(
            BoardCoordinates(_board->width() - 1, _board->height() - 1).xToString()))},
        _number_width{static_cast<uint8_t>(length(
            BoardCoordinates(_board->width() - 1, _board->height() - 1).yToString()))},
        _gap{"   "},
        _grid_width{_number_width + 1 + (1 + _letter_width) * _board->width() + 1},
        _width{_grid_width * 2 + _gap.size()},
        _map_key{createBoatsKey()} {}

  ConsoleBoardGenDisplay(const ConsoleBoardGenDisplay&)                  = default;
  ConsoleBoardGenDisplay(ConsoleBoardGenDisplay&&)                       = default;
  ConsoleBoardGenDisplay& operator=(const ConsoleBoardGenDisplay& other) = delete;
  ConsoleBoardGenDisplay& operator=(ConsoleBoardGenDisplay&&)            = delete;

  /** prints a temporary screen when changing turns, until next '\n' (enter) */
  void printChangeTurn() override;

  /** Produces a redraw */
  void update() override;

  /** Parse coordinates provided by user, check boundaries and call
   * BoardControl::fire. */
  void handleInput() override;

  ~ConsoleBoardGenDisplay() override = default;
};
