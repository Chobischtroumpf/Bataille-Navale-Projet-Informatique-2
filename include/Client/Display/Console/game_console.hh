#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "console.hh"
#include "game_client.hh"
#include "ship_classic.hh"
#include "game_controller.hh"
#include "not_implemented_error.hh"

enum InputStatus { OK, ERR };

enum GamePhase { PLACE_SHIP, WAIT_GAME, GAME, WAIT_TURN, END_GAME, SPECTATE };

/** BoardDisplay using text.
 *
 * A grid is a side of the board as represented on the screen.
 * Draw both sides of the board as two grids side by side. */
class GameConsole : public Console {
private:
  /** ConsoleBoardDisplay's methods must check they support this */
  std::ostream &_out;                                //< Where to print
  std::istream &_in;                                 //< Where to read
  std::shared_ptr<LocalBoardCommander> const _board; //< What to print
  std::shared_ptr<GameController> const
      _control; //< Who to inform of user actions
  std::shared_ptr<GameClient> const
      _game_client; //< Who to inform of user actions

  uint8_t const _letter_width; //< Number of character in a column name
  uint8_t const _number_width; //< Number of character in a row name

  bool _spectating; //< True if the player is spectating

  std::string const _gap;   //< The string used as grid separator
  size_t const _grid_width; //< The number of character in a line of a grid
  size_t const _width; //< The number of character in a line with two grids and a gap
  std::vector<string> _map_key;  //< The map key, each string is a line without the ending '\n'
  bool _my_turn; //< True if it's the player's turn
  InputStatus _last_input = OK;  //< True if the last input was valid
  GamePhase _phase; //< The current phase of the game
  bool _ship_selected = false; //< True if a boat has been selected
  std::unique_ptr<ShipClassic> _possible_ships = nullptr;                 //< The ships that can be placed

  std::string _round_time = ""; //< The time of the round
  std::string _game_time = "";  //< The time of the game

  // Place ship variables
  int _ship_size =
      0;            //< The size of the ship to place, 0 means no selected ship
  int _ship_id = 0; //< The id of the ship to place
  // Utility methods

  /** Count number of unicode characters in a UTF-8 encoded string (assume linux
   * platform)
   */
  static constexpr size_t length(const string &s);

  // Pre-print methods

  /** Single character to represent a type of cell on the board */
  static string toString(CellType type) {
    switch (type) {
    case WATER:
      return " ";
    case OCEAN:
      return "╳";
    case SCANNED_SHIP:
    case UNDAMAGED_SHIP:
      return "█";
    case HIT_SHIP:
      return "▒";
    case SUNK_SHIP:
      return "░";
    case UNDAMAGED_MINE:
    case SCANNED_MINE:
      return "¤";
    case HIT_MINE:
      return "⚑";
    default:
      throw NotImplementedError("ConsoleBoardDisplay unknown CellType");
    }
  }

  /** Create a header indicating who should play */
  [[nodiscard]] string createGameHeader() const;

  /** Create a header indicating who is setting the boat */
  [[nodiscard]] string createPlaceShipHeader() const;

  /** Create a grid label: Your / Their Fleet */
  [[nodiscard]] string createGridLabel(bool my_side) const;

  /** Create grid for a given side, each string is a line without ending '\n' */
  [[nodiscard]] std::vector<string> createGrid(bool my_side) const;

  /** Create map key, each string is a line without ending '\n'
   * Used by the constructor to init _map_key.
   * Outside constructor, use the attribute instead. */
  [[nodiscard]] std::vector<string> createMapKey() const;

  [[nodiscard]] std::vector<string> createBoatsKey() const;
  std::vector<string> createPlaceShipKey() const;

  /** Create coordinates prompt, each string is a line without ending '\n'.
   * Empty lines are added in the beginning of the prompt so it can be printed
   * next to the map key. */
  [[nodiscard]] std::vector<string> createGamePrompt(InputStatus status) const;

  // Print methods

  /** Print a vector of lines adding '\n' at the end of each line */
  void print(const std::vector<string> &lines);

  /** clear fail bits of _in, ignore until next '\n', redraw with the code
   * status*/
  void clearBadPlaceShipInput(bool placed);

  /** handle respectively the selection of the ship size, the ship selection and
   * the ship placement*/
  void handleShipSize();
  void handleShipSelection();
  void handleShipPlacement();

  bool isValidInputFormat(const std::string &input) const;
  bool isValidCoordinates(size_t row, size_t col) const;

  std::vector<string> createSelectShipSizePrompt(InputStatus status) const;
  std::vector<string> createSelectNextRotateKey() const;
  std::vector<string> createSelectShipPositionPrompt(InputStatus status) const;
  std::vector<string> createAvailableAbilities() const;
  std::vector<string> createAvailableBoats() const;

public:
  /** @param out: where to print
   * @param in: from where to read
   * @param board: what to print, must inherit from BoardView
   * @param control: who to inform of user actions.
   *
   * WARNING: ConsoleBoardDisplay does not support a change in board->height()
   * or board->width() after construction. */
  GameConsole(std::ostream &out, std::istream &in,
              std::shared_ptr<LocalBoardCommander> board,
              std::shared_ptr<GameController> control,
              std::shared_ptr<GameClient> client,
              bool spectating = false);

  GameConsole(const GameConsole &) = default;
  GameConsole() = delete;
  GameConsole(GameConsole &&) = default;
  GameConsole &operator=(const GameConsole &other) = delete;
  GameConsole &operator=(GameConsole &&) = delete;

  /** prints a temporary screen when changing turns, until next '\n' (enter) */
  void printChangeTurn();

  /**
   * Print on the screen two elements side by side
   **/
  void printSideBySide(std::vector<string> left, std::vector<string> right);

  /*
   * Print on the screen three elements side by side
   */
  void printThreeElements(std::vector<string> left, std::vector<string> middle,
                          std::vector<string> right);
  void printCenter(const std::vector<string> &board);

  /** Produces a redraw */
  void updatePlaceShip(InputStatus status);
  void updateGame(InputStatus);
  void displayWaitGame();
  void displayWaitTurn();
  void displayEndGame();
  void displaySpectating();

  /** Parse coordinates provided by user, check boundaries and call
   * BoardControl::fire. */
  ReturnInput handleFire();
  ReturnInput handlePlaceShip();
  ReturnInput handleQuit();

  // Console methods
  void display() override;
  void displayError() override;
  void update() override;
  ReturnInput handleInput() override;

  virtual ~GameConsole() = default;
};
