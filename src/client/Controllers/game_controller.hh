#pragma once

#include "../display.hh"
#include "../controller.hh"
#include "../local_board.hh"
#include <memory>
#include <utility>

/** Interface to implement to receive events from the display */
class GameController : public Controller {
public:
  GameController(std::shared_ptr<LocalBoard> board);
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
  virtual bool fire(BoardCoordinates coord) const;

  /* Handle the placement of a ship */
  virtual bool placeShip(ShipCoordinates coord) const;

  /* Send the ships to the server */
  virtual void sendShips(std::vector<ShipCoordinates> boats);

  virtual void connectServer();


  /** Inform that the player quit the game. */
  virtual void quit();

  // Make destructor virtual
  virtual ~GameController() = default;
  
private:
  std::shared_ptr<LocalBoard> _board;
  std::shared_ptr<Display> _display;

  virtual bool checkShipPosition(ShipCoordinates coord) const;
  virtual bool checkShipsInBoard(ShipCoordinates coord) const;
  bool sendRequest(ShipCoordinates coord);
  bool sendRequest(BoardCoordinates coord);
};
