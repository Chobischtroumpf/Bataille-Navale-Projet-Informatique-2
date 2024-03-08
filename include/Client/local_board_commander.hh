#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <cstdint>

#include "board_coordinates.hh"
#include "cell.hh"
#include "not_implemented_error.hh"
#include "ship_commander.hh"
#include "game_view.hh"


/*
 * Local board view
 */
class LocalBoardCommander : public GameView {
public:
  LocalBoardCommander();
  
  virtual ~LocalBoardCommander() override = default;

  // Getters
  bool myTurn() const override ;
  bool isFinished() const override;
  bool isVictory() const override;
  std::size_t width() const override;
  std::size_t height() const override;

  /*
   * Get the cell type at the given coordinates
    * @param coordinates : coordinates of the cell
    * @return le cell type
  */
  CellType cellType(bool my_side, BoardCoordinates coordinates) const override;

  /* WARNING : NOT FULLY IMPLEMENTED 
   * Check if two cells are part of the same ship*/
  bool isSameShip(bool my_side, BoardCoordinates first,
                  BoardCoordinates second) const override;

  /* Get the neighbors of a cell */
  std::vector<Cell> getNeighbors(BoardCoordinates coord) const;

  /* Returns the ships that needs can be placed */
  std::array<uint8_t, 5> shipsToPlace() const;

  /* Returns true if all boats are placed */
  bool allBoatsPlaced() const;

  /* Returns true if the ship is remaining */
  bool isRemainingShip(int number_of_case) const;

  /* Returns the ships that needs can be placed */
  std::vector<Ship> getPlacedShips() const;

  /* Add a place a ship (locally) */
  void addPlacedShip(Ship ship, int x, int y);

  /* Polls the server to wait the beggining of the game */
  void waitGame();

  /* Polls the server to wait the turn */
  void waitTurn();

  void update() override { throw NotImplementedError("Update"); }

  CellType best(CellType lhs, CellType rhs);


private:
  std::vector<std::vector<Cell>> _my_board;
  std::vector<Ship> _placed_ships;
  std::vector<std::vector<Cell>> _their_board;
  std::array<uint8_t, 5> _ships_to_place = {1, 1, 2, 1, 1};
  bool _my_turn;
  bool _is_finished;
  bool _is_victory;

  /* Get the cell in one of the board*/
  Cell get(bool my_side, BoardCoordinates position) const;

  int shipId(bool my_side, BoardCoordinates position);
  bool check();
  //void placeShip(ShipCoordinates coordinates, bool my_fleet);
  void fire();
};