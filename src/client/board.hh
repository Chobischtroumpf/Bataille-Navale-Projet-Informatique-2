#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <random>

#include "board_control.hh"
#include "board_display.hh"
#include "board_view.hh"
#include "board_coordinates.hh"

using std::nullopt;
using std::vector;


class Board final : public BoardView {

    /** The cell type and an optional ship identifier */
    class Cell {
        CellType           _type{WATER};
        std::optional<int> _ship_id{nullopt};

        public:
            /** Default constructor: creates an unkown/Water cell */
            constexpr Cell() = default;
            constexpr Cell(CellType type, std::optional<int> ship_id)
                : _type{type}, _ship_id{std::forward<std::optional<int>>(ship_id)} {
            if (type & IS_SHIP && !_ship_id) {
                throw std::logic_error("Cell with ship but no ship_id");
            } else if (!(type & IS_SHIP) && _ship_id) {
                throw std::logic_error("Cell without ship but with ship_id");
            }
            }
            // Public method to set the type of the cell
            void setType(CellType newType) {
                _type = newType;
            }
            [[nodiscard]] constexpr inline CellType           type() const { return _type; }
            [[nodiscard]] constexpr inline std::optional<int> shipId() const { return _ship_id; }
    };

    struct ShipCoordinates {
        BoardCoordinates anchor;
        int length;
        bool vertical;

        // Constructor for initialization
        ShipCoordinates(const BoardCoordinates& anchor, int length, bool vertical)
            : anchor(anchor), length(length), vertical(vertical) {}
    };
    
    class Ship {
        public:
            bool state{false}; // True for operational, false for sunk

            void notify(const BoardCoordinates& coords) {
                // Check if coords is one of the ship's tiles
                for (const BoardCoordinates& tile : tiles) {
                    if (tile == coords) {
                        std::cout << "Ship hit at coordinates (" << coords.x() << ", " << coords.y() << ")!\n";
                        return;
                    }
                }
            }

           Ship(const ShipCoordinates& coords) {
            for (int i = 0; i < coords.length; i++) {
                tiles[i] = BoardCoordinates{(coords.anchor.x() + (!coords.vertical ? i : 0)),
                                            (coords.anchor.y() + (coords.vertical? i : 0))};
            }

        }

        private:
            static constexpr size_t MAX_TILES = 10; // A limit to the maximum size of ships
            BoardCoordinates tiles[MAX_TILES];
};


    

  std::weak_ptr<BoardDisplay> _display{};
  bool                        _my_turn{true};
  bool                        _is_finished{false};
  bool                        _is_victory{false};

  vector<vector<Cell>> _my_side{
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
  };

  vector<vector<Cell>> _their_side{
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {SUNK, 2}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {OCEAN, nullopt}, {}, {}, {}, {}, {}, {HIT, 3}, {HIT, 3}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {OCEAN, nullopt}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}},
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

 public:
    Board() {
       
        //testBoard(); // Call the method to test the board methods

    }

    // method to generate the board
    void testBoard() {
    
        ShipCoordinates boat{BoardCoordinates{2, 3}, 3, false};

        placeShip(boat);

        simplePrint(_my_side);

        fire(BoardCoordinates{2, 3});

        simplePrint(_my_side);
    }

    // Method to place a ship on the board
    void placeShip(ShipCoordinates& boatcoord) {
        
        for ( int i = 0; i < boatcoord.length; i++) {
            _my_side[boatcoord.anchor.y() + ( boatcoord.vertical ? i  : 0 )][boatcoord.anchor.x() + ( !boatcoord.vertical ? i : 0 )].setType(UNDAMAGED);
        }
    }

    void fire(const BoardCoordinates& coords){
        Cell& cell = _my_side[coords.y()][coords.x()];
        if (cell.type() == UNDAMAGED) {
            cell.setType(HIT);
        }
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

  void setDisplay(std::weak_ptr<BoardDisplay> display) {
    _display = std::forward<std::weak_ptr<BoardDisplay>>(display);
    if (auto p = _display.lock()) {
      p->update();
    }
  }

  [[nodiscard]] bool     myTurn() const override { return _my_turn; }
  [[nodiscard]] bool     isFinished() const override { return _is_finished; }
  [[nodiscard]] bool     isVictory() const override { return _is_victory; }
  [[nodiscard]] size_t   width() const override { return _my_side.at(0).size(); }
  [[nodiscard]] size_t   height() const override { return _my_side.size(); }
  [[nodiscard]] CellType cellType(bool             my_side,
                                  BoardCoordinates position) const override {
    return get(my_side, position).type();
  }
  [[nodiscard]] bool isSameShip(bool my_side, BoardCoordinates first,
                                BoardCoordinates second) const override {
    return shipId(my_side, first).has_value() &&
           shipId(my_side, first) == shipId(my_side, second);
  }
  
  ~Board() override = default;
};
