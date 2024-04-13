#pragma once

#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "board_coordinates.hh"
#include "cell_type.hh"
#include "game_view.hh"

class Ship {
  private:
    std::vector<BoardCoordinates> _coordinates;
    BoardCoordinates _top_left;
    CellType _type = UNDAMAGED_SHIP;
    GameView *_board;
    size_t _length = 0;
    size_t _size_x = 0;
    size_t _size_y = 0;
    bool _is_sunk = false; // peut être juste changer le celltype ?

    void setSunk(bool is_sunk);

  public:
    // Constructors
    Ship(std::vector<BoardCoordinates> coordinates);
    Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates);
    Ship(std::vector<BoardCoordinates> coordinates, GameView *board);
    Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates, GameView *board);
    Ship(const Ship &other);

    // Operators
    Ship &operator=(const Ship &other);
    bool operator==(const Ship &other) const;


    // Getters
    std::vector<BoardCoordinates> getCoordinates() const;
    BoardCoordinates getTopLeft() const;
    // const std::vector<std::vector<Cell>> getShipCells() const;
    CellType getType() const;
    int getLength() const;
    int getSizeX() const;
    int getSizeY() const;
    bool isSunk() const;

    //Setter

    // void setShipCells(const vector<vector<Cell>> &ship_cells);
    void setType(CellType new_type);
    void setTopLeft(BoardCoordinates top_left);

    // Notifies the Ship that a tile has been hit, allowing it to check if it has been sunk
    void notify();

    // Translate the ship by x and y   
    bool translate(int x, int y);

    // Does a 90° rotation of the ship
    void rotate();

    // Checks if the coordinates passed as argument are part of the ship
    bool isPartOfShip(BoardCoordinates coord);

    // Print the ship
    std::vector<std::string> to_string();
    nlohmann::json to_json();

    // Destructor
    ~Ship();
};
