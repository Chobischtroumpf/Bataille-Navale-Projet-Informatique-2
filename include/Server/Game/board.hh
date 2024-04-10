#pragma once

#include <nlohmann/json.hpp>
#include <optional>

#include "player.hh"
#include "cell.hh"
#include "ship.hh"
#include "turn.hh"
#include "player_role.hh"
#include "board_coordinates.hh"
#include "big_torpedo_iterator.hh"
#include "aerial_strike_iterator.hh"
#include "piercing_torpedo_iterator.hh"

using std::map;
using std::nullopt;
using std::vector;
using json = nlohmann::json;

/*
 * Représente le plateau de jeu des deux joueurs
 */
class Board: public virtual GameView {
  private:
    bool _my_turn; // true if it's playerone turn, false if it's not
    Player _player1;
    Player _player2;
    bool _fleetA_state;
    bool _fleetB_state;

    std::vector<std::vector<Cell>>  _player1_side;
    std::vector<std::vector<Cell>>  _player2_side;

    // updates the board after a cell has been hit
    void setHit(BoardCoordinates coords);

    // updates the board after a torpedo has been fired
    void fireBigTorpedo(BoardCoordinates coords);

    // updates the board after a piercing torpedo has been fired
    void firePiercingTorpedo(BoardCoordinates coords);

    // updates the board after an aerial strike has been fired
    void fireAerialStrike(BoardCoordinates coords);

    // dispatches the torpedo to the correct method
    void dispatchTorpedo(SpecialAbilityType ability_type, BoardCoordinates coords);

    // updates the board after it has been scanned
    void setScanned(BoardCoordinates coords);

    // updates the board after a sonar has been fired
    void fireSonar(BoardCoordinates coords);

    // updates the board after a probe has been fired
    void fireProbe(BoardCoordinates coords);

    // dispatches the sonar to the correct method
    void dispatchSonar(SpecialAbilityType ability_type, BoardCoordinates coords);

    // updates the board after a mine has been set
    void setMine(SpecialAbility ability, BoardCoordinates coords);

  public:
    Board();
    Board(Player player1, Player player2);
    Board(const Board &) = delete;
    Board(Board &&) = delete;
    Board &operator=(const Board &) = delete;
    Board &operator=(Board &&) = delete;
    ~Board() = default;

    size_t width() const override;
    size_t height() const  override;

    Player& getPlayer1();
    Player& getPlayer2();

    // method called after a player has finished his turn
    void changeTurn();

    // method that returns true only if one of the player's fleet is set as destroyed
    bool isFinished() const override;

    // method to call only at the end of the game, true if playerone won, false if playertwo won
    bool isVictory() const override;

    // returns the player whose turn it is
    Turn whoseTurn() const;

    // adds a ship to the board
    void placeShip(Ship ship, bool side);

    // notifies the opponent's board that the cell in coord has been hit
    void notify(const BoardCoordinates &coords);

    // method to call when a player wants to fire a special ability
    void fire(SpecialAbility ability, BoardCoordinates coords);

    // converts a CellType to a string
    static string to_string(CellType type) {
      switch (type) {
      case WATER:
        return "WATER";
      case OCEAN:
        return "OCEAN";
      case SCANNED:
        return "SCANNED";
      case UNDAMAGED_MINE:
        return "UNDAMAGED_MINE";
      case SCANNED_MINE:
        return "SCANNED_MINE";
      case HIT_MINE:
        return "HIT_MINE";
      case UNDAMAGED_SHIP:
        return "UNDAMAGED_SHIP";
      case SCANNED_SHIP:
        return "SCANNED_SHIP";
      case HIT_SHIP:
        return "HIT_SHIP";
      case SUNK_SHIP:
        return "SUNK_SHIP";
      default:
        throw NotImplementedError("Board unknown CellType");
      }
    }

    // converts the board to a json object
    nlohmann::json to_json(PlayerRole role) const;

    // returns the cell at the given position
    Cell get(bool my_side, BoardCoordinates position) const;

    // returns the type of the cell at the given position
    CellType cellType(bool my_side, BoardCoordinates position) const;

    // returns the ship at the given position
    std::optional<Ship> shipId(bool my_side, BoardCoordinates position) const;

    // returns true if the two given positions are part of the same ship
    bool isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const  override;

    bool isInBoard(BoardCoordinates coord) const;

    // Iterators methods
    // Big Torpedo
    BigTorpedoIterator beginBigTorpedo(BoardCoordinates coords);

    BigTorpedoIterator endBigTorpedo(BoardCoordinates coords);

    // Piercing Torpedo
    PiercingTorpedoIterator beginPiercingTorpedo(BoardCoordinates coords);

    PiercingTorpedoIterator endPiercingTorpedo(BoardCoordinates coords);

    // Aerial Strike
    AerialStrikeIterator beginAerialStrike(BoardCoordinates coords);

    AerialStrikeIterator endAerialStrike(BoardCoordinates coords);
};
