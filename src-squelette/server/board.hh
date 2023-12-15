#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <map>
#include <random>

#include "board.hh"
#include "../common/board_coordinates.hh"
#include "../common/ship_coordinates.hh"
#include "ship.hh"
#include "../common/turn.hh"
#include "../common/ship_types.hh"
#include "game_server.hh"

using std::nullopt;
using std::vector;
using std::map;

class Board {

    /** The cell type and an optional ship identifier */
    class Fleet {
        public:
            // Constructor to initialize the Fleet with ships
            Fleet(vector<vector<Cell>>& board) : _board{board},  _state{true} {}

            // Method to notify the Fleet that a Ship has sunk
            void notify(const BoardCoordinates& coords) {
                // Check if any ship in the fleet is operational
                _state = false;

                for (Ship& ship : _ships) {
                    ship.notify(coords);
                }

                for (const Ship& ship : _ships) {
                    //std::cout << "Here's the ship state : " << ship.getState() << std::endl;
                    if ( ship.getState() ) {
                        _state = true;
                        return;
                    }
                }
            }
            // Method to get the number of ships in the fleet
            map<ShipType, int> getNumShips() const {
                map<ShipType, int> numShips;
                for (const Ship& ship : _ships) {
                    numShips[ship.getType()]++;
                }
                return numShips;
            }

            // Method to get the state of the fleet (true if operational, false if sunk)
            bool getState() const {
                return _state;
            }

            // Method to add a ship to the fleet
            void addShip(const ShipCoordinates shipCoord) {
                _ships.push_back(Ship{shipCoord, _board});
            }

        private:
            const vector<vector<Cell>>& _board;
            std::vector<Ship> _ships;
            bool _state;
    };

  std::shared_ptr<GameServer> _server;
  bool                        _my_turn{true};
  bool                        _is_finished{false};
  bool                        _is_victory{false};

  vector<vector<Cell>> _my_side{
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}}
    };

  vector<vector<Cell>> _their_side{
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}}
    };

  [[nodiscard]] Cell get(bool my_side, BoardCoordinates position) const {
    if (my_side) {
      return _my_side.at(position.y()).at(position.x());
    } else {
      return _their_side.at(position.y()).at(position.x());
    }
  }

  [[nodiscard]] std::optional<int> shipId(bool my_side, BoardCoordinates position) const {
    return get(my_side, position).shipId();
  }

  [[nodiscard]] bool check() const {
    if (_my_side.size() != _their_side.size()) {
      return false;
    }
    for (size_t i = 0; i < height(); ++i) {
      if (_my_side.at(i).size() != width() || _their_side.at(i).size() != width()) {
        return false;
      }
    }
    return true;
  }

    Fleet _fleetA{_my_side}; // First fleet
    Fleet _fleetB{_their_side};  // Second fleet

 public:

    Board() {
        //testBoard(); // Call the method to test the board methods

    }

    map<ShipType, int> countShips(bool isA) const {
        return isA ? _fleetA.getNumShips() : _fleetB.getNumShips();
    }

    // Method to place a ship on the board
    void placeShip(ShipCoordinates& shipCoords, bool isA = true )  {
        if (myTurn()) {
            for (int i = 0; i < shipCoords.ship_id(); i++) {
                _my_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)][shipCoords.x() +
                                                                               (shipCoords.orientation() ? i
                                                                                                          : 0)].setType(
                        UNDAMAGED);
            }
        } else {
            for (int i = 0; i < shipCoords.ship_id(); i++) {
                _their_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)][shipCoords.x() +
                                                                               (shipCoords.orientation() ? i
                                                                                                          : 0)].setType(
                        UNDAMAGED);
            }

        }

        isA ? _fleetA.addShip(shipCoords) :  _fleetB.addShip(shipCoords) ;
    }

    void fire(const BoardCoordinates& coords){
        Cell& cell = myTurn() ? _their_side[coords.y()][coords.x()] : _my_side[coords.y()][coords.x()];
        if (cell.type() == UNDAMAGED) {
            cell.setType(HIT);
        } else if ( cell.type() == WATER) {
            cell.setType(OCEAN);
        }

        _fleetA.notify(coords);
        _fleetB.notify(coords);
    }
    
    void simplePrint(vector<vector<Cell>> board) {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                switch (cell.type()) {
                    case UNDAMAGED:
                        std::cout << "U ";
                        break;
                    case HIT:
                        std::cout << "H ";
                        break;
                    case OCEAN:
                        std::cout << "O ";
                        break;
                    default:
                        std::cout << "? ";
                        break;
                }
            }
            std::cout << std::endl;
        }
    }
        

  Board(const Board&)            = delete;
  Board(Board&&)                 = delete;
  Board& operator=(const Board&) = delete;
  Board& operator=(Board&&)      = delete;

  [[nodiscard]] bool     myTurn() const { return _my_turn; }

  //To call ater each turn to know if the game is over.
  [[nodiscard]] bool     isFinished() const { 
    return _fleetA.getState() and _fleetB.getState();
    }
    //To call after the game is over to know who won.
  [[nodiscard]] bool     isVictory() const { 
    return _fleetA.getState(); 
    }
    
  [[nodiscard]] size_t   width() const { return _my_side.at(0).size(); }
  [[nodiscard]] size_t   height() const { return _my_side.size(); }
  [[nodiscard]] CellType cellType(bool             my_side,
                                  BoardCoordinates position) const {
    return get(my_side, position).type();
  }
  [[nodiscard]] bool isSameShip(bool my_side, BoardCoordinates first,
                                BoardCoordinates second) const {
    return shipId(my_side, first).has_value() &&
           shipId(my_side, first) == shipId(my_side, second);
  }

    [[nodiscard]] std::vector<Cell> getNeighbors(BoardCoordinates coord) {
        std::vector<Cell> neighbors;
        bool my_side = myTurn() ? true : false;
        if (coord.x() > 0) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.y(), coord.x()-1)));
        }
        if (coord.x() < width()) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.y(), coord.x()+1)));
        }
        if (coord.y() > 0) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.y()-1, coord.x())));
            if (coord.x() > 0) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.y()-1, coord.x()-1)));
            }
            if (coord.x() < width()) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.y()-1, coord.x()+1)));
            }
        }
        if (coord.y() < height()) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.y()+1, coord.x())));
            // Diagonale gauche
            if (coord.x() > 0) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.y()+1, coord.x()-1)));
            }
            if (coord.x() < width()) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.y()+1, coord.x()+1)));
            }
        }
        return neighbors;
    }
  void changeTurn() {_my_turn = !_my_turn;}
  
  Turn whoseTurn() const {
      if (myTurn()) {
          return PLAYERONE;
      } else {
          return PLAYERTWO;
      }
  }
  
  ~Board() = default;
};
