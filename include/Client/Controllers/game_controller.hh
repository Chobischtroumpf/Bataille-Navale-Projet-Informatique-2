#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <utility>

#include "board_coordinates.hh"
#include "controller.hh"
#include "display.hh"
#include "local_board_commander.hh"
#include "ship.hh"

/** Interface to implement to receive events from the display */
class GameController : public Controller {
  private:
    std::shared_ptr<LocalBoardCommander> _board;
    std::shared_ptr<Display> _display;

    virtual bool checkShipPosition(Ship coord) const;
  public:
    GameController(std::shared_ptr<LocalBoardCommander> board);
    GameController(const GameController &) = default;
    GameController(GameController &&) = default;
    GameController &operator=(const GameController &) = default;
    GameController &operator=(GameController &&) = default;

    virtual void setDisplay(std::shared_ptr<Display> display) {
      _display = std::move(display);
    }

    /** Inform that the player chose to fire on this cell.
     * Return true if the action is valid (this cell was not targeted previously).
     */
    virtual bool fire(SpecialAbility ability, BoardCoordinates coord) const;

    /* Handle the placement of a ship */
    virtual bool placeShip(Ship ship) const;

    /** Inform that the player quit the game. */
    virtual void quit();

    // Make destructor virtual
    virtual ~GameController() = default;
};
