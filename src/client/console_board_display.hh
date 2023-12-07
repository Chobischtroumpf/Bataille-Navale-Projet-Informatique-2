#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

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
class ConsoleBoardDisplay final : public BoardDisplay {
  /** ConsoleBoardDisplay's methods must check they support this */
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
  static string toString(BoardView::CellType type) {
    switch (type) {
      case BoardView::WATER:
        return " ";
      case BoardView::OCEAN:
        return "╳";
      case BoardView::UNDAMAGED:
        return "█";
      case BoardView::HIT:
        return "▒";
      case BoardView::SUNK:
        return "░";
      default:
        throw NotImplementedError("ConsoleBoardDisplay unknown CellType");
    }
  }

  /** Create a header indicating who should play */
  [[nodiscard]] string createHeader() const;

  /** Create a grid label: Your / Their Fleet */
  [[nodiscard]] string createGridLabel(bool my_side) const;

  /** Create grid for a given side, each string is a line without ending '\n' */
  [[nodiscard]] vector<string> createGrid(bool my_side) const;

  /** Create map key, each string is a line without ending '\n'
   * Used by the constructor to init _map_key.
   * Outside constructor, use the attribute instead. */
  [[nodiscard]] static vector<string> createMapKey() {
    vector<string> map_key;
    map_key.emplace_back(" > " + toString(BoardView::OCEAN) + " Ocean          <");
    map_key.emplace_back(" > " + toString(BoardView::UNDAMAGED) + " Undamaged ship <");
    map_key.emplace_back(" > " + toString(BoardView::HIT) + " Hit ship       <");
    map_key.emplace_back(" > " + toString(BoardView::SUNK) + " Sunk ship      <");
    return map_key;
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
  ConsoleBoardDisplay() = delete;

  /** @param out: where to print
   * @param in: from where to read
   * @param board: what to print, must inherit from BoardView
   * @param control: who to inform of user actions.
   *
   * WARNING: ConsoleBoardDisplay does not support a change in board->height() or
   * board->width() after construction. */
  ConsoleBoardDisplay(std::ostream& out, std::istream& in,
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
        _map_key{createMapKey()} {}

  ConsoleBoardDisplay(const ConsoleBoardDisplay&)                  = default;
  ConsoleBoardDisplay(ConsoleBoardDisplay&&)                       = default;
  ConsoleBoardDisplay& operator=(const ConsoleBoardDisplay& other) = delete;
  ConsoleBoardDisplay& operator=(ConsoleBoardDisplay&&)            = delete;

  /** Produces a redraw */
  void update() override;

  void printChangeTurn() override;

  /** Parse coordinates provided by user, check boundaries and call
   * BoardControl::fire. */
  void handleInput() override;

  ~ConsoleBoardDisplay() override = default;
};
