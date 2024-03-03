#pragma once

#include "board.hh"
#include "../../../include/common/board_coordinates.hh"
#include "../../../include/common/ship_coordinates.hh"
#include <memory>
#include <utility>

/** Interface to implement to receive events from the display */
class BoardControl {
private:
    std::shared_ptr<Board> _board;

    /** controls if the ship is already in the Board */
    virtual bool _checkShipsInBoard(ShipCoordinates coord);

    /** controls if all of the Cell's the ship will use are correct */
    virtual bool _checkShipPosition(ShipCoordinates coord);
public:
  BoardControl(const BoardControl&)            = default;
  BoardControl(BoardControl&&)                 = default;
  BoardControl& operator=(const BoardControl&) = default;
  BoardControl& operator=(BoardControl&&)      = default;
  BoardControl(std::shared_ptr<Board> board) : _board{std::move(board)} {}

  /** Inform that the player chose to fire on this cell.
   * Return true if the action is valid (this cell was not targeted previously). */
  virtual bool fire(BoardCoordinates coord);
  
  /** controls if the ship's position is acceptable,
   * and places it in the Board if it is*/
  virtual bool placeShip(ShipCoordinates coord);

  /** Inform that the player quit the game. */
  virtual void quit();


  // Make destructor virtual
  virtual ~BoardControl() = default;
};
