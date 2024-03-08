#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include "board_coordinates.hh"

class Ship {
  private:
    std::vector<BoardCoordinates> _coordinates;
    int _size_x = 0;
    int _size_y = 0;
    int _number_of_case = 0;

  public:
    Ship(std::vector<BoardCoordinates> coordinates);
    void rotate();
    void print();
    std::vector<BoardCoordinates> getCoordinates();
    int getNumberOfCase();
};
