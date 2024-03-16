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
  

  //Fleet _fleetA{_player1_side};    // First fleet
  //Fleet _fleetB{_player2_side}; // Second fleet
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

  void changeTurn(){
    _turn = !_turn;
  }

  bool isFinished() const {
    return !(_fleetA_state && _fleetB_state);
  }

  bool isVictory() const { return _fleetA_state; }

  Turn whoseTurn() const {
    if (myTurn()){
      return PLAYERONE;
    }else{
      return PLAYERTWO;
    }
  }

  void placeShip(Ship &ship, bool side) {
    if (side) {
      for (auto& board_coordinates:ship.getCoordinates()){
        _player1_side[board_coordinates.y()][board_coordinates.x()].setType(UNDAMAGED_SHIP);
        _player1.addShip(ship);
      }

    } else {
      for (auto& board_coordinates:ship.getCoordinates()){
        _player2_side[board_coordinates.y()][board_coordinates.x()].setType(UNDAMAGED_SHIP);
        _player2.addShip(ship);
      }
    }

  }

  void notify(const BoardCoordinates &coords) {
    // Iterate over the enemy ships
    Player& current_player = myTurn() ? _player2:_player1;

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

  void fire(SpecialAbility ability, BoardCoordinates coords) {

    Player& current_player = myTurn() ? _player1 : _player2;
    if (current_player.getEnergyPoints() < ability.getEnergyCost()) {
        return; // Not enough energy points to use the ability
    }

    current_player.removeEnergyPoints(ability.getEnergyCost());

    Cell &cell = myTurn() ? _player2_side[coords.y()][coords.x()]
                          : _player1_side[coords.y()][coords.x()];
    if (cell.type() == UNDAMAGED_SHIP) {
      cell.setType(HIT_SHIP);
    } else if (cell.type() == WATER) {
      cell.setType(OCEAN);
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

  nlohmann::json to_json(PlayerRole player) const {
    nlohmann::json boardJson;
    nlohmann::json fleetAJson, fleetBJson;
    for (size_t y = 0; y < height(); ++y) {
      nlohmann::json rowJsonA, rowJsonB;
      for (size_t x = 0; x < width(); ++x) {
        nlohmann::json cellObjectA, cellObjectB;
        Cell cellA, cellB;
        if (player == PlayerRole::Leader or player == PlayerRole::Spectator) {
          cellA = _player1_side[y][x];
          cellB = _player2_side[y][x];
        } else {
          cellA = _player2_side[y][x];
          cellB = _player1_side[y][x];
        }

        cellObjectA["type"] = to_string(cellA.type());
        rowJsonA.push_back(cellObjectA);

        if ((player == PlayerRole::Leader || player == PlayerRole::Opponent) &&
            cellB.type() == UNDAMAGED_SHIP) {
          cellObjectB["type"] = to_string(WATER);
        } else {
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

  CellType cellType(bool my_side,
                                  BoardCoordinates position) const {
    return get(my_side, position).type();
  }

  std::optional<Ship> shipId(bool my_side,
                                          BoardCoordinates position) const {
    return get(my_side, position).ship();
  }

  bool isSameShip(bool my_side, BoardCoordinates first,
                                BoardCoordinates second) const  override {
    return shipId(my_side, first).has_value() &&
           shipId(my_side, first) == shipId(my_side, second);
  }

};
