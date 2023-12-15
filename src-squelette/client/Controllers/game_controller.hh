#pragma once

#include "../../common/board_coordinates.hh"
#include "../../common/ship_coordinates.hh"
#include "../display.hh"
#include "../controller.hh"
#include "../local_board.hh"
#include <memory>
#include <utility>

/** Interface to implement to receive events from the display */
class GameController : public Controller {
private:
  std::shared_ptr<Display> _display;
  std::shared_ptr<LocalBoard> _board;

  virtual bool checkShipPosition(ShipCoordinates coord) const;
  bool sendRequest(ShipCoordinates coord);
  bool sendRequest(BoardCoordinates coord);

public:
  GameController(const GameController &) = default;
  GameController(GameController &&) = default;
  GameController &operator=(const GameController &) = default;
  GameController &operator=(GameController &&) = default;
  GameController();

  virtual void setDisplay(std::shared_ptr<Display> display) override {
    _display = std::move(display);
  }

  /** Inform that the player chose to fire on this cell.
   * Return true if the action is valid (this cell was not targeted previously).
   */
  virtual bool fire(BoardCoordinates coord) const override;

  virtual bool placeShip(ShipCoordinates coord) const override;

  /** Inform that the player quit the game. */
  virtual void quit();

  // Make destructor virtual
  virtual ~GameController() = default;
};
