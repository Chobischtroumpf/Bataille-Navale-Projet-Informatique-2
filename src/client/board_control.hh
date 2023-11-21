#pragma once

#include "board_coordinates.hh"

/** Interface to implement to receive events from the display */
class BoardControl {
 protected:
  BoardControl(const BoardControl&)            = default;
  BoardControl(BoardControl&&)                 = default;
  BoardControl& operator=(const BoardControl&) = default;
  BoardControl& operator=(BoardControl&&)      = default;

 public:
  BoardControl() = default;

  /** Inform that the player chose to fire on this cell.
   * Return true if the action is valid (this cell was not targeted previously). */
  virtual bool fire(BoardCoordinates) = 0;
  /** Inform that the player quit the game. */
  virtual void quit()                 = 0;

  // Make destructor virtual
  virtual ~BoardControl() = default;
};
