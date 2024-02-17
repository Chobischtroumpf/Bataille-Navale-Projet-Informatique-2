#pragma once

#include <cstddef>
#include <vector>
#include <map>
#include <iostream>

#include "../common/board_coordinates.hh"
#include "../common/cell.hh"
#include "../common/not_implemented_error.hh"
#include "../common/ship_coordinates.hh"
#include "Views/game_view.hh"


/*
 * Copie local du board afin de pouvoir l'afficher en jeu
 */
class LocalBoard : public GameView {
public:
  LocalBoard()
      : _is_finished{false}, _is_victory{false}, _my_turn{true},
        _my_board{std::vector<std::vector<Cell>>(10, std::vector<Cell>(10, Cell()))} {}
  
  virtual ~LocalBoard() override = default;

  bool myTurn() override { return _my_turn; }
  bool isFinished() override { return _is_finished; }
  bool isVictory() override { return _is_victory; }
  std::size_t width() override { return _my_board.at(0).size(); }
  std::size_t height() override { return _my_board.size(); }
  CellType cellType(bool my_side, BoardCoordinates coordinates) override {
    return get(my_side, coordinates).type();
  }
  bool isSameShip(bool my_side, BoardCoordinates first,
                  BoardCoordinates second) override;
  std::vector<Cell> getNeighbors(BoardCoordinates coord) const;

  void update() override { throw NotImplementedError("Update"); }

  static constexpr inline CellType best(CellType lhs, CellType rhs) {
    if (!(lhs & IS_SHIP) || !(rhs & IS_SHIP)) {
      std::cerr << "BoardView::best(" << static_cast<unsigned>(lhs) << ", "
                << static_cast<unsigned>(rhs) << ")" << std::endl;
      throw std::logic_error("BoardView::best called with non-ship types");
    }
    return lhs <= rhs ? lhs : rhs;
  }

  std::map<ShipType, uint8_t> countShips(bool my_side) const;

private:
  std::vector<std::vector<Cell>> _my_board;
  bool _my_turn;
  bool _is_finished;
  bool _is_victory;

  Cell get(bool my_side, BoardCoordinates position);
  int shipId(bool my_side, BoardCoordinates position);
  bool check();
  void placeShip(ShipCoordinates coordinates, bool my_fleet);
  void fire();
};