#pragma once

#include <nlohmann/json.hpp>
#include <optional>

#include "player.hh"
#include "cell.hh"
#include "board_coordinates.hh"
#include "ship.hh"
#include "turn.hh"
#include "player_role.hh"


using std::map;
using std::nullopt;
using std::vector;
using json = nlohmann::json;

/*
 * Représente le plateau de jeu des deux joueurs
 */
class Board: public GameView {
  private:
    bool _my_turn{true}; // true if it's playerone turn, false if it's not
    Player _player1{};
    Player _player2{};
    bool _fleetA_state{true};
    bool _fleetB_state{true};

    std::vector<std::vector<Cell>>  _player1_side{{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}}};

    std::vector<std::vector<Cell>>  _player2_side{{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
                                                  {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}}};

    class BigTorpedoIterator {
      private:
        BoardCoordinates _coords;
        size_t _i;
        size_t _j;

      public:
        BigTorpedoIterator(BoardCoordinates coords, size_t width, size_t height)
            : _coords{coords}, _i{0}, _j{0} {}

        BoardCoordinates operator*() const { return _coords + BoardCoordinates(_i, _j); }

        BigTorpedoIterator &operator++() {
          if (_j < 1) {
            ++_j;
          } else {
            _j = 0;
            ++_i;
          }
          return *this;
        }

        bool operator==(const BigTorpedoIterator &other) const {
          return _i == other._i && _j == other._j;
        }

        bool operator!=(const BigTorpedoIterator &other) const {
          return _i + _coords.x() != other._i + other._coords.x() || _j + _coords.y() != other._j + other._coords.y();
        }
    };

    class PiercingTorpedoIterator {
      private:
        BoardCoordinates _coords;
        size_t _i;

      public:
        PiercingTorpedoIterator(BoardCoordinates coords, size_t width, size_t height)
            : _coords{coords}, _i{0} {}

        BoardCoordinates operator*() const { return _coords + BoardCoordinates(0, _i); }

        PiercingTorpedoIterator &operator++() {
          ++_i;
          return *this;
        }

        bool operator==(const PiercingTorpedoIterator &other) const { return _i == other._i; }

        bool operator!=(const PiercingTorpedoIterator &other) const {
          return _i + _coords.y() != other._i + other._coords.y();
        }
    };

    class AerialStrikeIterator {
      private:
        BoardCoordinates _coords;
        size_t _i;
        size_t _j;
        bool _top_right{false};

      public:
        AerialStrikeIterator(BoardCoordinates coords, size_t width, size_t height)
            : _coords{coords}, _i{0}, _j{0} {}

        BoardCoordinates operator*() const { return _coords + BoardCoordinates(_i, _j); }

        AerialStrikeIterator &operator++() {
          if (!_top_right) {
            if (_i < 3) {
              _i++;
            } else if (_j < 2) {
              _j++;
            } else {
              _top_right = true;
              _i = 0;
              _j = 1;
            }
          } else {
            if (_j < 3) {
              _j++;
            } else {
              _i++;
            }
          }
          return *this;
        }

        bool operator==(const AerialStrikeIterator &other) const {
          return _i == other._i && _j == other._j;
        }

        bool operator!=(const AerialStrikeIterator &other) const {
          return _i + _coords.x() != other._i + other._coords.x() || _j + _coords.y() != other._j + other._coords.y();
        }
    };

    // updates the board after a cell has been hit
    void setHit(BoardCoordinates coords) {
      Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                            : _player1_side[coords.y()][coords.x()];
      Player &current_player = _my_turn ? _player1 : _player2;
      Player &opponent = _my_turn ? _player2 : _player1;

      if (cell.type() == UNDAMAGED_SHIP) {
        cell.setType(HIT_SHIP);
      } else if (cell.type() == UNDAMAGED_MINE) {
        cell.setType(HIT_MINE);
        if (opponent.getFaction().getName() == "Mines"){
          current_player.removeEnergyPoints(3);
        } else {
          current_player.removeEnergyPoints(2);
        }
      } else if (cell.type() == WATER) {
        cell.setType(OCEAN);
      }
    }

    // updates the board after a torpedo has been fired
    void fireBigTorpedo(BoardCoordinates coords) {
      for (BigTorpedoIterator it = beginBigTorpedo(coords);
           it != endBigTorpedo(coords); ++it) {
            if (isInBoard(*it)){
              setHit(*it);
            }
      }
    }

    // updates the board after a piercing torpedo has been fired
    void firePiercingTorpedo(BoardCoordinates coords) {
      for (PiercingTorpedoIterator it = beginPiercingTorpedo(coords); it != endPiercingTorpedo(coords); ++it) {
            if (isInBoard(*it)){
              setHit(*it);
            }
      }
    }

    // updates the board after an aerial strike has been fired
    void fireAerialStrike(BoardCoordinates coords) {
      for (AerialStrikeIterator it = beginAerialStrike(coords); it != endAerialStrike(coords); ++it) {
        if (isInBoard(*it)) {
          setHit(*it);
        }
      }
    }

    // dispatches the torpedo to the correct method
    void dispatchTorpedo(SpecialAbilityType ability_type, BoardCoordinates coords) {
      
      if (ability_type == TORPEDO)
        setHit(coords);
      else if (ability_type == BIG_TORPEDO)
        fireBigTorpedo(coords);
      else if (ability_type == PIERCING_TORPEDO)
        firePiercingTorpedo(coords);
      else if (ability_type == AERIAL_STRIKE)
        fireAerialStrike(coords);
    }

    // updates the board after it has been scanned
    void setScanned(BoardCoordinates coords) {
      Cell &cell = _my_turn ? _player2_side[coords.y()][coords.x()]
                            : _player1_side[coords.y()][coords.x()];
      if (cell.type() == UNDAMAGED_SHIP) {
        cell.setType(SCANNED_SHIP);
      } else if (cell.type() == UNDAMAGED_MINE) {
        cell.setType(SCANNED_MINE);
      } else if (cell.type() == WATER) {
        cell.setType(SCANNED);
      }
    }

    // updates the board after a sonar has been fired
    void fireSonar(BoardCoordinates coords) {
      for (size_t i = 0; i < 3; i++){
        for (size_t j = 0; j < 3; j++){
          setScanned(BoardCoordinates(i,j)+coords);
        }
      }
    }

    // updates the board after a probe has been fired
    void fireProbe(BoardCoordinates coords) {
      for (int i = this->width(); i > 0; i--)
        setScanned(BoardCoordinates(i+1, coords.y()));
    }

    // dispatches the sonar to the correct method
    void dispatchSonar(SpecialAbilityType ability_type, BoardCoordinates coords) {
      if (ability_type == SONAR)
        fireSonar(coords);
      else if (ability_type == PROBE)
        fireProbe(coords);
    }

    void setMine(SpecialAbility ability, BoardCoordinates coords) {
      Player& current_player = _my_turn ? _player1 : _player2;
      current_player.addShip(Ship(coords, {{0,0}}, this));
    }
  public:
    Board(){};
    Board(Player player1, Player player2): _player1{player1}, _player2{player2} {}
    Board(const Board &) = delete;
    Board(Board &&) = delete;
    Board &operator=(const Board &) = delete;
    Board &operator=(Board &&) = delete;
    ~Board() = default;


    // bool myTurn() const { return _my_turn; }
    size_t width() const override{ return _player1_side.at(0).size(); }
    size_t height() const  override{ return _player1_side.size(); }

    Player& getPlayer1() { return _player1; }
    Player& getPlayer2() { return _player2; }

    void changeTurn(){ _my_turn = !_my_turn; _player1.swapTurn();_player2.swapTurn();}

    // method that returns true only if one of the player's fleet is set as destroyed
    bool isFinished() const { return !(_fleetA_state && _fleetB_state); }

    // method to call only at the end of the game, true if playerone won, false if playertwo won
    bool isVictory() const { return _fleetA_state; }

    // returns the player whose turn it is
    Turn whoseTurn() const {
      if (_my_turn){
        return PLAYERONE;
      }else{
        return PLAYERTWO;
      }
    }

    // adds a ship to the board
    void placeShip(Ship ship, bool side) {
      std::vector<std::vector<Cell>> &board = side ? _player1_side : _player2_side;
      Player &player = side ? _player1 : _player2;
      for (auto& board_coordinates : ship.getCoordinates()){
        board[ship.getTopLeft().y() + board_coordinates.y()][ship.getTopLeft().x() + board_coordinates.x()].setType(ship.getType());
      }
      player.addShip(ship);
    }

    // notifies the opponent's board that the cell in coord has been hit
    void notify(const BoardCoordinates &coords) {
      // Iterate over the enemy's ships
      Player& current_player = _my_turn ? _player2 : _player1;

      for (Ship &ship : current_player.getFleet()) {
        ship.notify(coords);
        if (ship.getType() != UNDAMAGED_MINE && !ship.isSunk()) {
          return;
        }
      }
      if (_my_turn){
        _fleetB_state = false;
      } else{
        _fleetA_state = false;
      }
    }

    // method to call when a player wants to fire a special ability
    void fire(SpecialAbility ability, BoardCoordinates coords) {
      Player& current_player = _my_turn ? _player1 : _player2;
      if (current_player.getEnergyPoints() < ability.getEnergyCost()) {
          return; // Not enough energy points to use the ability
      }

      current_player.removeEnergyPoints(ability.getEnergyCost());
      
      if (ability.getType() & IS_TORPEDO) {
        dispatchTorpedo(ability.getType(), coords);
      } else if (ability.getType() & IS_SONAR) {
        fireSonar(coords);
      } else if (ability.getType() & MINE) {
        setMine(ability, coords);        
      }
      notify(coords);
    }

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
    nlohmann::json to_json(PlayerRole role) const {
      nlohmann::json boardJson;
      nlohmann::json fleetAJson, fleetBJson;
      for (size_t y = 0; y < height(); ++y) {
        nlohmann::json rowJsonA, rowJsonB;
        for (size_t x = 0; x < width(); ++x) {
          nlohmann::json cellObjectA, cellObjectB;
          Cell cellA, cellB;
          if (role == PlayerRole::Leader || role == PlayerRole::Spectator) {
            cellA = _player1_side[y][x];
            cellB = _player2_side[y][x];
          } else {
            cellA = _player2_side[y][x];
            cellB = _player1_side[y][x];
          }

          // always sets the true type of the cell in cellObjectA as it will always be the player's side 
          cellObjectA["type"] = to_string(cellA.type());
          rowJsonA.push_back(cellObjectA);

          // if player is leader or opponent it will hide the type of the cell as it is not the player's side
          if ((role == PlayerRole::Leader || role == PlayerRole::Opponent) &&
              (cellB.type() == UNDAMAGED_SHIP || cellB.type() == UNDAMAGED_MINE)) {
            cellObjectB["type"] = to_string(WATER);
          } else {
            // if player is a spectator it will show the type of the cell
            cellObjectB["type"] = to_string(cellB.type());
          }
          rowJsonB.push_back(cellObjectB);
        }
        fleetAJson.push_back(rowJsonA);
        fleetBJson.push_back(rowJsonB);
      }

      boardJson["fleetA"] = fleetAJson;
      boardJson["fleetB"] = fleetBJson;

      return boardJson;
    }

    // returns the cell at the given position
    Cell get(bool my_side, BoardCoordinates position) const {
      if (my_side) {
        return _player1_side.at(position.y()).at(position.x());
      } else {
        return _player2_side.at(position.y()).at(position.x());
      }
    }

    // returns the type of the cell at the given position
    CellType cellType(bool my_side, BoardCoordinates position) const {
      return get(my_side, position).type();
    }

    // returns the ship at the given position
    std::optional<Ship> shipId(bool my_side, BoardCoordinates position) const {
      return get(my_side, position).ship();
    }

    // returns true if the two given positions are part of the same ship
    bool isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const  override {
      return shipId(my_side, first).has_value() &&
            shipId(my_side, first) == shipId(my_side, second);
    }

    bool isInBoard(BoardCoordinates coord) const {
      return coord.x() >= 0 && coord.x() < width() && coord.y() >= 0 && coord.y() < height();
    }

    // Iterators methods
    // Big Torpedo
    BigTorpedoIterator beginBigTorpedo(BoardCoordinates coords) {
      return BigTorpedoIterator(coords, width(), height());
    }

    BigTorpedoIterator endBigTorpedo(BoardCoordinates coords) {
      return BigTorpedoIterator(coords + BoardCoordinates(2, 0), width(), height());
    }

    // Piercing Torpedo
    PiercingTorpedoIterator beginPiercingTorpedo(BoardCoordinates coords) {
      return PiercingTorpedoIterator(coords, width(), height());
    }

    PiercingTorpedoIterator endPiercingTorpedo(BoardCoordinates coords) {
      return PiercingTorpedoIterator(coords + BoardCoordinates(0, 4), width(), height());
    }

    // Aerial Strike
    AerialStrikeIterator beginAerialStrike(BoardCoordinates coords) {
      return AerialStrikeIterator(coords, width(), height());
    }

    AerialStrikeIterator endAerialStrike(BoardCoordinates coords) {
      return AerialStrikeIterator(coords + BoardCoordinates(4, 3), width(), height());
    }

};
