#pragma once

#include <nlohmann/json.hpp>

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
    bool _turn{true}; // true if it's playerone turn, false if it's not
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
  public:
    Board(){};
    Board(const Board &) = delete;
    Board(Board &&) = delete;
    Board &operator=(const Board &) = delete;
    Board &operator=(Board &&) = delete;
    ~Board() = default;

    bool myTurn() const {return _turn;}

    size_t width() const override{ return _player1_side.at(0).size(); }
    size_t height() const  override{ return _player1_side.size(); }

    void changeTurn(){ _turn = !_turn; }

    // method that returns true only if one of the player's fleet is set as destroyed
    bool isFinished() const { return !(_fleetA_state && _fleetB_state); }

    // method to call only at the end of the game, true if playerone won, false if playertwo won
    bool isVictory() const { return _fleetA_state; }

    // returns the player whose turn it is
    Turn whoseTurn() const {
      if (myTurn()){
        return PLAYERONE;
      }else{
        return PLAYERTWO;
      }
    }


    void placeShip(Ship ship, bool side) {
      std::vector<std::vector<Cell>> &board = side ? _player1_side : _player2_side;
      Player &player = side ? _player1 : _player2;
      for (auto& board_coordinates : ship.getCoordinates()){
        board[ship.getTopLeft().y() + board_coordinates.y()][ship.getTopLeft().x() + board_coordinates.x()].setType(ship.getType());
        player.addShip(ship);
      }
    }

    // notifies the opponent's board that the cell in coord has been hit
    void notify(const BoardCoordinates &coords) {
      // Iterate over the enemy's ships
      Player& current_player = myTurn() ? _player2 : _player1;

      for (Ship &ship : current_player.getFleet()) {
        ship.notify(coords);
        if (!ship.isSunk()){
          return;
        }
      }
      if (myTurn()){
        _fleetB_state = false;
      } else{
        _fleetA_state = false;
      }
    }

    // updates the board after a cell has been hit
    void setHit(BoardCoordinates coords) {
      Cell &cell = myTurn() ? _player2_side[coords.y()][coords.x()]
                            : _player1_side[coords.y()][coords.x()];
      Player &current_player = myTurn() ? _player1 : _player2;
      Player &opponent = myTurn() ? _player2 : _player1;

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

    // updates the board after it has been scanned
    void setScanned(BoardCoordinates coords) {
      Cell &cell = myTurn() ? _player2_side[coords.y()][coords.x()]
                            : _player1_side[coords.y()][coords.x()];
      if (cell.type() == UNDAMAGED_SHIP) {
        cell.setType(SCANNED_SHIP);
      } else if (cell.type() == UNDAMAGED_MINE) {
        cell.setType(SCANNED_MINE);
      } else if (cell.type() == WATER) {
        cell.setType(SCANNED);
      }
    }

    // updates the board after a one of the torpedos has been fired
    void fireTorpedo(SpecialAbility ability, BoardCoordinates coords) {
      std::vector<std::vector<Cell>> &board = myTurn() ? _player2_side : _player1_side;

      if (ability.getType() == TORPEDO) {
        setHit(coords);
      } else if (ability.getType() == BIG_TORPEDO) {
        for (int i =0; i<2 && coords.x() + i < width()-1; i++) {
          for (int j =0; j<2 && coords.y() + j < height()-1; j++) {
            setHit(BoardCoordinates(coords.x() + i, coords.y() + j));
          }
        }
      } else if (ability.getType() == PIERCING_TORPEDO) {
        for (int i = 0; i < 4 && coords.y() + i < height(); i++){
          setHit(BoardCoordinates(coords.x(), coords.y() + i));
        }
      } else if (ability.getType() == AERIAL_STRIKE) {
          for (int i = 0; i < 4 && coords.x() + i < width(); i++) {
            setHit(BoardCoordinates(coords.x() + i, coords.y()));
            if (coords.y() + 3 < height())
              setHit(BoardCoordinates(coords.x() + i, coords.y() + 3));
          }
          for (int j = 1; j < 3 && coords.y() + j < height(); j++) {
            setHit(BoardCoordinates(coords.x(), coords.y() + j));
            if (coords.x() + 3 < width())
              setHit(BoardCoordinates(coords.x() + 3, coords.y() + j));
        }
      }
    }

    // 
    void fireSonar(SpecialAbility ability, BoardCoordinates coords) {
      std::vector<std::vector<Cell>> &board = myTurn() ? _player1_side : _player2_side;
      if (ability.getType() == SONAR) {
        for (int i = 0; i < 3; i++){
          for (int j = 0; j < 3; j++){
            setScanned(BoardCoordinates(coords.x() + i, coords.y() + j));
          }
        }
      } else if (ability.getType() == PROBE) {
        for (int i = this->width(); i > 0; i--){
          setScanned(BoardCoordinates(i+1, coords.y()));
        }
      }
    }

    void setMine(SpecialAbility ability, BoardCoordinates coords) {
      Player& current_player = myTurn() ? _player1 : _player2;
      current_player.addShip(Ship(coords, {{0,0}}, this));
    }

    void fire(SpecialAbility ability, BoardCoordinates coords) {
      Player& current_player = myTurn() ? _player1 : _player2;
      if (current_player.getEnergyPoints() < ability.getEnergyCost()) {
          return; // Not enough energy points to use the ability
      }

      current_player.removeEnergyPoints(ability.getEnergyCost());

      if (ability.getType() == TORPEDO) {
        
      } else if (ability.getType() & IS_TORPEDO) {
        fireTorpedo(ability, coords);
      } else if (ability.getType() & IS_SONAR) {
        fireSonar(ability, coords);
      } else if (ability.getType() & MINE) {
        setMine(ability, coords);        
      }
      notify(coords);
    }

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

    Cell get(bool my_side, BoardCoordinates position) const {
      if (my_side) {
        return _player1_side.at(position.y()).at(position.x());
      } else {
        return _player2_side.at(position.y()).at(position.x());
      }
    }

    CellType cellType(bool my_side, BoardCoordinates position) const {
      return get(my_side, position).type();
    }

    std::optional<Ship> shipId(bool my_side, BoardCoordinates position) const {
      return get(my_side, position).ship();
    }

    bool isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const  override {
      return shipId(my_side, first).has_value() &&
            shipId(my_side, first) == shipId(my_side, second);
    }

};
