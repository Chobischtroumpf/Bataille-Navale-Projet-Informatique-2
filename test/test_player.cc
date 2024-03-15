#include "player.hh"
#include "Commandant/Factions/faction_classique.hh"
#include "Commandant/Factions/faction_bombardement.hh"
#include "Commandant/Factions/faction_mines.hh"
#include "Commandant/Factions/faction_sonar.hh"
#include <cassert>
#include <iostream>

void testPlayer() {
    std::cout << "Testing Player" << std::endl;
    // Test default constructor
    Player player1;
    assert(player1.getFaction() == FactionClassique());
    assert(player1.getFleet().empty());
    assert(player1.isTurn() == false);
    assert(player1.getEnergyPoints() == 0);
    std::cout << "Default constructor passed" << std::endl;

    // Test parameterized constructor
    Player player2 = Player(FactionBombardement());
    assert(player2.getFaction() == FactionBombardement());
    assert(player2.getFleet().empty());
    assert(player2.isTurn() == false);
    assert(player2.getEnergyPoints() == 0);
    std::cout << "Parameterized constructor passed" << std::endl;

    // Test copy constructor
    Player player3(player2);
    assert(player3.getFaction() == FactionBombardement());
    assert(player3.getFleet().empty());
    assert(player3.isTurn() == false);
    assert(player3.getEnergyPoints() == 0);
    std::cout << "Copy constructor passed" << std::endl;

    // Test assignment operator
    Player player4;
    player4 = player3;
    assert(player4.getFaction() == FactionBombardement());
    assert(player4.getFleet().empty());
    assert(player4.isTurn() == false);
    assert(player4.getEnergyPoints() == 0);
    std::cout << "Assignment operator passed" << std::endl;

    // Test setter methods
    player4.setFaction(FactionMines());
    assert(player4.getFaction() == FactionMines());

    std::vector<Ship> fleet = {Ship({BoardCoordinates()}), Ship({BoardCoordinates()})};
    player4.setFleet(fleet);
    assert(player4.getFleet() == fleet);

    player4.addShip(Ship({BoardCoordinates()}));
    assert(player4.getFleet().size() == 3);

    player4.setTurn(true);
    assert(player4.isTurn() == true);

    player4.setEnergyPoints(100);
    assert(player4.getEnergyPoints() == 100);
    std::cout << "Setter methods passed" << std::endl;
}

int main() {
    testPlayer();
    return 0;
}