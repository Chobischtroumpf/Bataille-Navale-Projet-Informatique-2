#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <map>
#include <random>

#include "board_control.hh"
#include "board_display.hh"
#include "board_view.hh"
#include "board.hh"
#include "board_coordinates.hh"
#include "ship_coordinates.hh"
#include "ship.hh"
#include "turn.hh"
#include "ship_types.hh"

using std::nullopt;
using std::vector;
using std::map;

class Board final : public BoardView {

    //Class representing a player's fleet
    class Fleet {
        public:
            // Constructor to initialize the Fleet with ships
            Fleet(vector<vector<Cell>>& board) : _board{board},  _state{true} {}

            // Method to notify the Fleet that a Ship has sunk
            void notify(const BoardCoordinates& coords) {
                // Check if any ship in the fleet is operational

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
                _state = false;
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

  std::weak_ptr<BoardDisplay> _display{};
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

    //Returns a value copy of a Cell of the board
  [[nodiscard]] Cell get(bool my_side, BoardCoordinates position) const {
    if (my_side) {
      return _my_side.at(position.y()).at(position.x());
    } else {
      return _their_side.at(position.y()).at(position.x());
    }
  }
    //Returns the ship id of the cell at given coordinates
  [[nodiscard]] std::optional<int> shipId(bool my_side, BoardCoordinates position) const {
    return get(my_side, position).shipId();
  }

    //Checks if the dimensions of the board are correct
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

    Board() {}

    map<ShipType, int> countShips(bool isA) const override{
        return isA ? _fleetA.getNumShips() : _fleetB.getNumShips();
    }

    //Places a ship on the board
    void placeShip(ShipCoordinates& shipCoords, bool isA = true )  {
        try {
        if (myTurn()) {
            for (int i = 0; i < shipCoords.ship_id(); i++) {
                _my_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)]
                        [shipCoords.x() + (shipCoords.orientation() ? i : 0)].setType(UNDAMAGED);

                _my_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)]
                        [shipCoords.x() + (shipCoords.orientation() ? i : 0)].setId(static_cast<int>(shipCoords.ship_id()));
            }
        } else {
            for (int i = 0; i < shipCoords.ship_id(); i++) {
                _their_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)]
                            [shipCoords.x() + (shipCoords.orientation() ? i : 0)].setType(UNDAMAGED);

                _their_side[shipCoords.y() + (!shipCoords.orientation() ? i : 0)]
                            [shipCoords.x() + (shipCoords.orientation() ? i : 0)].setId(static_cast<int>(shipCoords.ship_id()));       
            }
        }
        } catch (std::exception& e) {
            std::cout << "Exception caught : " << e.what() << std::endl;
            exit(1);
        }

        isA ? _fleetA.addShip(shipCoords) :  _fleetB.addShip(shipCoords) ;
    }

    //"Fires" at the given position on the board
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
        
  Board(const Board&)            = delete;
  Board(Board&&)                 = delete;
  Board& operator=(const Board&) = delete;
  Board& operator=(Board&&)      = delete;

  void setDisplay(std::weak_ptr<BoardDisplay> display) {
    _display = std::forward<std::weak_ptr<BoardDisplay>>(display);
    if (auto p = _display.lock()) {
        p->printChangeTurn();
        p->updateGame();
    }
  }

  [[nodiscard]] bool     myTurn() const override { return _my_turn; }

  //To call ater each turn to know if the game is over.
  [[nodiscard]] bool     isFinished() const override { 
    return !(_fleetA.getState() and _fleetB.getState());
    }
    //To call after the game is over to know who won.
  [[nodiscard]] bool     isVictory() const override { 
    return _fleetA.getState(); 
    }
    
  [[nodiscard]] size_t   width() const override { return _my_side.at(0).size(); }
  [[nodiscard]] size_t   height() const override { return _my_side.size(); }
  //Getter method to access a cell of the board
  [[nodiscard]] CellType cellType(bool             my_side,
                                  BoardCoordinates position) const override {
    return get(my_side, position).type();
  }
  //Checks if two board coordinates belong to the same ship
  [[nodiscard]] bool isSameShip(bool my_side, BoardCoordinates first,
                                BoardCoordinates second) const override {
    return shipId(my_side, first).has_value() &&
           shipId(my_side, first) == shipId(my_side, second);
  }
    //Returns a vector of all the cells adjacent to the given coordinates ( diagonals too )
    [[nodiscard]] std::vector<Cell> getNeighbors(BoardCoordinates coord) {
        std::vector<Cell> neighbors;
        bool my_side = myTurn() ? true : false;
        if (coord.x() > 0) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.x()-1, coord.y())));
        }
        if (coord.x()+1 < width()) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.x()+1, coord.y())));
        }
        if (coord.y() > 0) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.x(), coord.y()-1)));
            if (coord.x() > 0) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.x()-1, coord.y()-1)));
            }
            if (coord.x()+1 < width()) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.x()+1, coord.y()-1)));
            }
        }
        if (coord.y()+1 < height()) {
            neighbors.push_back(get(my_side, BoardCoordinates(coord.x(), coord.y()+1)));
            // Diagonale gauche
            if (coord.x() > 0) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.x()-1, coord.y()+1)));
            }
            if (coord.x()+1 < width()) {
                neighbors.push_back(get(my_side, BoardCoordinates(coord.x()+1, coord.y()+1)));
            }
        }
        return neighbors;
    }

  void changeTurn() {_my_turn = !_my_turn;}
  
  Turn whoseTurn() const override {
      if (myTurn()) {
          return PLAYERONE;
      } else {
          return PLAYERTWO;
      }
  }
  
  ~Board() override = default;
};
