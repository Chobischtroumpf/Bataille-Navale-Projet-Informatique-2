#pragma once

#include <memory>

#include "Commandant/Factions/faction.hh"
#include "Commandant/Factions/faction_classique.hh"

class Player {
  private:
    Faction _faction;
    std::vector<Ship> _fleet;
    bool _is_turn;
    int _energy_points;
  
  public:
    Player();
    Player(Faction faction);
    virtual ~Player() = default;

    Player(const Player& other);
    Player& operator=(const Player& other);

    Faction getFaction() const;
    std::vector<Ship> getFleet() const;
    bool isTurn() const;
    int getEnergyPoints() const;

    void setFaction(Faction faction);
    void setFleet(std::vector<Ship> fleet); // Cette méthode est peut etre inutile ?
    void addShip(Ship ship);
    void setTurn(bool is_turn);
    void setEnergyPoints(int energy_points);
};
