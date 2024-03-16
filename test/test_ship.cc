#include <cassert>
#include <iostream>
#include "local_board_commander.hh"
#include "ship.hh"

void testShip() {
    std::cout << "Testing Ship" << std::endl;

    // Test constructor with coordinates
    std::vector<BoardCoordinates> coordinates = {BoardCoordinates(0, 0), BoardCoordinates(0, 1)};
    Ship ship1(coordinates);
    assert(ship1.getCoordinates() == coordinates);
    assert(ship1.getTopLeft() == BoardCoordinates(0, 0));
    assert(ship1.getType() == UNDAMAGED_SHIP);
    assert(ship1.getLength() == 2);
    assert(ship1.getSizeX() == 1);
    assert(ship1.getSizeY() == 2);
    assert(ship1.isSunk() == false);
    std::cout << "Constructor with coordinates passed" << std::endl;

    // Test constructor with coordinates and board
    LocalBoardCommander *board = new LocalBoardCommander(std::make_shared<GameClient>("http://127.0.0.1:8000"), Player(), GameMode::CLASSIC, "session_id");
    std::vector<BoardCoordinates> coordinates2 = {BoardCoordinates(0, 0), BoardCoordinates(0, 1)};
    Ship ship2(coordinates2, board);
    assert(ship2.getCoordinates() == coordinates);
    assert(ship2.getTopLeft() == BoardCoordinates(0, 0));
    assert(ship2.getType() == UNDAMAGED_SHIP);
    assert(ship2.getLength() == 2);
    std::cout << "SizeX: " << ship2.getSizeX() << std::endl;
    assert(ship2.getSizeX() == 1);
    assert(ship2.getSizeY() == 2);
    assert(ship2.isSunk() == false);
    std::cout << "Constructor with coordinates and board passed" << std::endl;

    // Test copy constructor
    Ship ship3(ship1);
    assert(ship3.getCoordinates() == coordinates);
    assert(ship3.getTopLeft() == BoardCoordinates(0, 0));
    assert(ship3.getType() == UNDAMAGED_SHIP);
    assert(ship3.getLength() == 2);
    assert(ship3.getSizeX() == 1);
    assert(ship3.getSizeY() == 2);
    assert(ship3.isSunk() == false);
    std::cout << "Copy constructor passed" << std::endl;

    // Test assignment operator
    Ship ship4(coordinates);
    ship4 = ship3;
    assert(ship4.getCoordinates() == coordinates);
    assert(ship4.getTopLeft() == BoardCoordinates(0, 0));
    assert(ship4.getType() == UNDAMAGED_SHIP);
    assert(ship4.getLength() == 2);
    assert(ship4.getSizeX() == 1);
    assert(ship4.getSizeY() == 2);
    assert(ship4.isSunk() == false);
    std::cout << "Assignment operator passed" << std::endl;

    // Test rotate method
    ship4.rotate();
    // Add assertions for rotated ship

    // Test notify method
    BoardCoordinates hitCoords(0, 0);
    ship4.notify(hitCoords);
    // Add assertions for updated ship state

    // Test to_string method
    std::vector<std::string> shipString = ship4.to_string();
    // Add assertions for ship string representation

    std::cout << "All Ship tests passed" << std::endl;
}

int main() {
    testShip();
    return 0;
}