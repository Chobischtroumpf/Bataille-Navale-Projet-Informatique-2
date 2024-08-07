#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include "board_coordinates.hh"
#include "cell_type.hh"
#include "game_view.hh"

#define PLAYER1 true
#define PLAYER2 false

class Ship {
  private:
    std::vector<BoardCoordinates> _coordinates{};
    BoardCoordinates _top_left{};
    CellType _type{UNDAMAGED_SHIP};
    GameView *_board;
    size_t _length{0};
    size_t _size_x{0};
    size_t _size_y{0};
    bool _is_sunk{false}; // peut être juste changer le celltype ?
    bool _side{PLAYER1};

    void setSunk(bool is_sunk);

  public:
    // Constructors
    Ship(std::vector<BoardCoordinates> coordinates);
    Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates, bool _side = PLAYER1);
    Ship(std::vector<BoardCoordinates> coordinates, GameView *board);
    Ship(BoardCoordinates top_left, std::vector<BoardCoordinates> coordinates, GameView *board, bool _side = PLAYER1);
    Ship(const Ship &other);

    // Operators
    Ship &operator=(const Ship &other);
    bool operator==(const Ship &other) const;
    bool operator==(const Ship *other) const;


    // Getters
    std::vector<BoardCoordinates> getCoordinates() const;
    bool isAt(const BoardCoordinates &coords) const;
    BoardCoordinates getTopLeft() const;
    // const std::vector<std::vector<Cell>> getShipCells() const;
    CellType getType() const;
    GameView *getBoard() const;
    size_t getLength() const;
    size_t getSizeX() const;
    size_t getSizeY() const;
    bool isSunk() const;
    bool getSide() const;

    //Setter

    // void setShipCells(const vector<vector<Cell>> &ship_cells);
    void setType(CellType new_type);
    void setTopLeft(BoardCoordinates top_left);
    void setBoard(GameView *board);

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
