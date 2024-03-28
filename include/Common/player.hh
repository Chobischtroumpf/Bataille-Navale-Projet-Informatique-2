#pragma once

#include <memory>

#include "faction.hh"
#include "faction_classique.hh"

class Player {
  private:
    Faction _faction;
    std::vector<Ship> _fleet;
    bool _is_turn;
    int _energy_points;
    bool _is_player_one;
  
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
    bool isPlayerOne() const;

    void setFaction(Faction faction);
    void setFleet(std::vector<Ship> fleet); // Cette méthode est peut etre inutile ?
    void addShip(Ship ship);
    void setTurn(bool is_turn);
    void swapTurn();
    void addEnergyPoints(int energy_points);
    void removeEnergyPoints(int energy_points);
    void setPlayerOne(bool is_player_one);
};
