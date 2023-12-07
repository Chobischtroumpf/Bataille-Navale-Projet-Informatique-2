#pragma once

#include "board_coordinates.hh"
#include "board.hh"
#include "board_display.hh"
#include <memory>
#include <utility>

/** Interface to implement to receive events from the display */
class BoardControl {
private:
    std::shared_ptr<Board> _board;
    std::shared_ptr<BoardDisplay> _display;
public:
  BoardControl(const BoardControl&)            = default;
  BoardControl(BoardControl&&)                 = default;
  BoardControl& operator=(const BoardControl&) = default;
  BoardControl& operator=(BoardControl&&)      = default;
  BoardControl(std::shared_ptr<Board> board) : _board{std::move(board)} {}

  virtual void setDisplay(std::shared_ptr<BoardDisplay> display) {_display = std::move(display);}

  /** Inform that the player chose to fire on this cell.
   * Return true if the action is valid (this cell was not targeted previously). */
  virtual bool fire(BoardCoordinates coord);
  /** Inform that the player quit the game. */
  virtual void quit();


  // Make destructor virtual
  virtual ~BoardControl() = default;
};
