#pragma once

#include <cstddef>
#include <vector>

#include "../common/board_coordinates.hh"
#include "../common/cell.hh"
#include "../common/not_implemented_error.hh"
#include "../common/ship_coordinates.hh"
#include "Views/game_view.hh"

using std::vector;

/*
 * Copie local du board afin de pouvoir l'afficher en jeu
 */
class LocalBoard : public GameView {
public:
  LocalBoard()
      : _is_finished{false}, _is_victory{false}, _my_turn{true},
        _my_side{vector<vector<Cell>>(10, vector<Cell>(10, Cell()))},
        _their_side{vector<vector<Cell>>(10, vector<Cell>(10, Cell()))} {}

  bool myTurn() override { return _my_turn; }
  bool isFinished() override { return _is_finished; }
  bool isVictory() override { return _is_victory; }
  std::size_t width() override { return _my_side.at(0).size(); }
  std::size_t height() override { return _my_side.size(); }
  CellType cellType(bool my_side, BoardCoordinates coordinates) override {
    return get(my_side, coordinates).type();
  }
  bool isSameShip(bool my_side, BoardCoordinates first,
                  BoardCoordinates second) override;
  vector<Cell> getNeighbors(BoardCoordinates coord) const;

  void update() override { throw NotImplementedError("Update"); }

private:
  vector<vector<Cell>> _my_side;
  vector<vector<Cell>> _their_side;
  bool _my_turn;
  bool _is_finished;
  bool _is_victory;

  Cell get(bool my_side, BoardCoordinates position);
  int shipId(bool my_side, BoardCoordinates position);
  bool check();
  void placeShip(ShipCoordinates coordinates, bool my_fleet);
  void fire();
};