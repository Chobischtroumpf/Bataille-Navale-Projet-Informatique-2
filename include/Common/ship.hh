#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "board_coordinates.hh"
#include "cell_type.hh"
#include "game_view.hh"
#include "cell.hh"

using std::vector;

class Ship {
  private:
    vector<BoardCoordinates> _coordinates;
    BoardCoordinates _top_left;
    CellType _type = UNDAMAGED_SHIP;
    int _length = 0;
    int _size_x = 0;
    int _size_y = 0;
    bool _is_sunk = false; // peut être juste changer le celltype ?

    void setSunk(bool is_sunk);

  public:
    Ship(vector<BoardCoordinates> coordinates, std::shared_ptr<GameView> board = nullptr);

    Ship(const Ship &other);

    Ship &operator=(const Ship &other);

    // Does a 90° rotation of the ship
    void rotate();

    // Getters
    vector<BoardCoordinates> getCoordinates() const;
    BoardCoordinates getTopLeft() const;
    // const vector<vector<Cell>> getShipCells() const;
    CellType getType() const;
    int getLength() const;
    int getSizeX() const;
    int getSizeY() const;
    bool isSunk() const;

    //Setter

    void setShipCells(const vector<vector<Cell>> &ship_cells);

    // Notifies the Ship that a tile has been hit, allowing it to check if it has been sunk
    void notify(const BoardCoordinates &coords);


    // Print the ship
    void print();
    std::vector<BoardCoordinates> getCoordinates();
    BoardCoordinates getTopLeft();
    int getNumberOfCase();
    void setType(CellType new_type);
};
