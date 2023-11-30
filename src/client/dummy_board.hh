#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "board_control.hh"
#include "board_display.hh"
#include "board_view.hh"

using std::nullopt;
using std::vector;

/** A mock game board used to test the display.
 *
 * The DummyBoard use 2D vectors of DummyBoard::Cell to store hardcoded sides of the
 * board. This is convenient for a mock game board, but maybe not for a real game.
 *
 * Since this is a temporary class, the code quality is low. */
class DummyBoard final : public BoardView, public BoardControl {
  /** The cell type and an optional ship identifier */
  class Cell {
    CellType           _type{WATER};
    std::optional<int> _ship_id{nullopt};

   public:
    /** Default constructor: creates an unkown/Water cell */
    constexpr Cell() = default;
    constexpr Cell(CellType type, std::optional<int> ship_id)
        : _type{type}, _ship_id{std::forward<std::optional<int>>(ship_id)} {
      if (type & IS_SHIP && !_ship_id) {
        throw std::logic_error("Cell with ship but no ship_id");
      } else if (!(type & IS_SHIP) && _ship_id) {
        throw std::logic_error("Cell without ship but with ship_id");
      }
    }
    [[nodiscard]] constexpr inline CellType           type() const { return _type; }
    [[nodiscard]] constexpr inline std::optional<int> shipId() const { return _ship_id; }
  };

  std::weak_ptr<BoardDisplay> _display{};
  bool                        _my_turn{true};
  bool                        _is_finished{false};
  bool                        _is_victory{false};

  vector<vector<Cell>> _my_side{
      {{}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 2}, {UNDAMAGED, 2}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {OCEAN, nullopt}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {HIT, 3}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {HIT, 3}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {HIT, 3}, {}, {}, {}, {}, {}, {OCEAN, nullopt}, {}, {}},
      {{}, {}, {UNDAMAGED, 3}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {OCEAN, nullopt}, {}, {UNDAMAGED, 4}, {HIT, 4}, {HIT, 4}, {HIT, 4}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{SUNK, 6}, {SUNK, 6}, {SUNK, 6}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
  };
  vector<vector<Cell>> _their_side{
      {{UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {UNDAMAGED, 1}, {}, {UNDAMAGED, 1}, {}, {UNDAMAGED, 1}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {UNDAMAGED, 1}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {OCEAN, nullopt}, {}, {}, {}, {}, {}, {HIT, 3}, {HIT, 3}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {OCEAN, nullopt}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
  };

  [[nodiscard]] Cell get(bool my_side, BoardCoordinates position) const {
    if (my_side) {
      return _my_side.at(position.y()).at(position.x());
    } else {
      return _their_side.at(position.y()).at(position.x());
    }
  }

  [[nodiscard]] std::optional<int> shipId(bool my_side, BoardCoordinates position) const {
    return get(my_side, position).shipId();
  }

  [[nodiscard]] bool check() const {
    if (_my_side.size() != _their_side.size()) {
      return false;
    }
    for (size_t i = 0; i < height(); ++i) {
      if (_my_side.at(i).size() != width() || _their_side.at(i).size() != width()) {
        return false;
      }
    }
    return true;
  }

 public:
  DummyBoard() {
    if (!check()) {
      std::cerr << "The hard-coded DummyBoard is not valid" << std::endl;
      std::abort();
    }
  }
  DummyBoard(const DummyBoard&)            = delete;
  DummyBoard(DummyBoard&&)                 = delete;
  DummyBoard& operator=(const DummyBoard&) = delete;
  DummyBoard& operator=(DummyBoard&&)      = delete;

  void setDisplay(std::weak_ptr<BoardDisplay> display) {
    _display = std::forward<std::weak_ptr<BoardDisplay>>(display);
    if (auto p = _display.lock()) {
      p->update();
    }
  }

  [[nodiscard]] bool     myTurn() const override { return _my_turn; }
  [[nodiscard]] bool     isFinished() const override { return _is_finished; }
  [[nodiscard]] bool     isVictory() const override { return _is_victory; }
  [[nodiscard]] size_t   width() const override { return _my_side.at(0).size(); }
  [[nodiscard]] size_t   height() const override { return _my_side.size(); }
  [[nodiscard]] CellType cellType(bool             my_side,
                                  BoardCoordinates position) const override {
    return get(my_side, position).type();
  }
  [[nodiscard]] bool isSameShip(bool my_side, BoardCoordinates first,
                                BoardCoordinates second) const override {
    return shipId(my_side, first).has_value() &&
           shipId(my_side, first) == shipId(my_side, second);
  }

  [[nodiscard]] bool fire(const BoardCoordinates position) override {
    if (cellType(!_my_turn, position) & BoardView::IS_KNOWN) {
      std::cerr << "DummyBoard received an INvalid fire target: " << position << '\n';
      return false;  // Invalid target
    } else {
      std::cerr << "DummyBoard received a valid fire target: " << position << '\n';
      _my_turn = !_my_turn;
      if (auto p = _display.lock()) {
        p->printChangeTurn();
        p->update();
      }
      return true;
    }
  }
  void quit() override {
    _is_finished = true;
    _is_victory  = false;
  }

  ~DummyBoard() override = default;
};
