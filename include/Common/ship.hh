#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include "board_coordinates.hh"

class Ship {
  private:
    std::vector<std::pair<int, int>> _coordinates;
    BoardCoordinates _top_left;
    int _size_x = 0;
    int _size_y = 0;


  public:
    Ship(std::vector<std::pair<int, int>> coordinates);
    void rotate();
    std::vector<std::pair<int, int>> getCoordinates();
    void print();
};
