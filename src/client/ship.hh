#pragma once

#include <vector>
#include "board_coordinates.hh"
#include "ship_coordinates.hh"
#include "board_view.hh"
#include "cell.hh"

using std::vector;

class Ship {

    public:

        //Notifies the Ship that a tile has been hit, allowing it to check if it is sunk
        void notify(const BoardCoordinates& coords) {

            _state = false;

            // Check if ship is sunk
            for (const BoardCoordinates& tile : tiles) {
                //std::cout << "Checking for tile X : " << tile.x() << " Y : " << tile.y() << std::endl;
                
                if ( _board[tile.y()][tile.x()].type() == UNDAMAGED ) {
                    _state = true;
                    return;
                }
            }
        }

        Ship(const ShipCoordinates& coords, const vector<vector<Cell>>& board ) : _state{true}, _board{board} {
            for (int i = 0; i < coords.ship_id(); i++) {
                tiles[i] = BoardCoordinates{(coords.x() + (!coords.orientation() ? i : 0)),
                                            (coords.y() + (coords.orientation()? i : 0))};
            }
        }

        // Method to get the state of the ship (true if operational, false if sunk)
        bool getState() const {
            return _state;
        }

    private:
        bool _state;
        const vector<vector<Cell>>& _board;
        static constexpr size_t MAX_TILES = 10; // A limit to the maximum size of ships
        BoardCoordinates tiles[MAX_TILES];
};