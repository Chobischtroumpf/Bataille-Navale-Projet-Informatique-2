#pragma once

#include <optional>
#include <string>
#include <vector>

class Ship {
private:
    std::vector<std::pair<int, int>> _coordinates;
    int _size_x = 0;
    int _size_y = 0;

public:
    Ship() {};
    Ship(std::vector<std::pair<int, int>> coordinates);
    void addCoordinate(int x, int y);
    void rotate();
    std::vector<std::pair<int, int>> getCoordinates();
};
