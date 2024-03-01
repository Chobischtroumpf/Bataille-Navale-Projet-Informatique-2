#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "../common/board_coordinates.hh"
#include "../common/cell_type.hh"
#include "../common/ship_types.hh"
#include "../common/turn.hh"

using json = nlohmann::json;

/*
 * Ce qui est affiché à l'écran
 */
class BoardView {
protected:
  BoardView(const BoardView &) = default;
  BoardView(BoardView &&) = default;
  BoardView &operator=(const BoardView &) = default;
  BoardView &operator=(BoardView &&) = default;

public:
  /** The type of a board cell */

  /** Given two ship states, return the best one */
  static constexpr inline CellType best(CellType lhs, CellType rhs) {
    if (!(lhs & IS_SHIP) || !(rhs & IS_SHIP)) {
      std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
                << static_cast<unsigned>(rhs) << ")" << std::endl;
      throw std::logic_error("BoardView::best called with non-ship types");
    }
    return lhs <= rhs ? lhs : rhs;
  }

  BoardView() = default;

  /** Should I play or wait for the other player to play? */
  [[nodiscard]] virtual bool myTurn() const = 0;
  /** Is the game finished? */
  [[nodiscard]] virtual bool isFinished() const = 0;
  /** Am I the winner? */
  [[nodiscard]] virtual bool isVictory() const = 0;
  /** Valid BoardCoordinates' x are: 0 <= x < width() */
  [[nodiscard]] virtual size_t width() const = 0;
  /** Valid BoardCoordinates' y are: 0 <= y < height() */
  [[nodiscard]] virtual size_t height() const = 0;
  /** Must return the known cell type */
  [[nodiscard]] virtual CellType cellType(bool my_side,
                                          BoardCoordinates position) const = 0;
  /** Return true if both cells contain the same ship, false otherwise.
   * If one or both cells does not contain a ship, return false.
   * ATTENTION: the display should not reveal whether two cells contain parts of
   * the same ship if the two cells are not adjacent. */
  [[nodiscard]] virtual bool isSameShip(bool my_side, BoardCoordinates first,
                                        BoardCoordinates second) const = 0;

  virtual std::map<ShipType, int> countShips(bool isA) const = 0;

  virtual Turn whoseTurn() const = 0;

  [[nodiscard]] virtual json toJson(Turn turn) const = 0;

  [[nodiscard]] virtual json toJsonSpectator() const = 0;

  // Make destructor virtual
  virtual ~BoardView() = default;
};
