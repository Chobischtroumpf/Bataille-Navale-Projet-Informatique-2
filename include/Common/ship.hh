#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iostream>

class Ship {
  private:
    std::vector<std::pair<int, int>> _coordinates;
    int _size_x = 0;
    int _size_y = 0;

  public:
    Ship(std::vector<std::pair<int, int>> coordinates);
    void rotate();
    std::vector<std::pair<int, int>> getCoordinates();
    void print();
};
