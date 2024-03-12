#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>

#include "board_coordinates.hh"
#include "cell_type.hh"

class Ship {
  private:
    std::vector<BoardCoordinates> _coordinates;
    BoardCoordinates _top_left;
    int _size_x = 0;
    int _size_y = 0;
    int _number_of_case = 0;
    CellType _type = UNDAMAGED_SHIP;

  public:
    Ship(std::vector<BoardCoordinates> coordinates);
    void rotate();
    void print();
    std::vector<BoardCoordinates> getCoordinates();
    BoardCoordinates getTopLeft();
    int getNumberOfCase();
    void setType(CellType new_type);
};
