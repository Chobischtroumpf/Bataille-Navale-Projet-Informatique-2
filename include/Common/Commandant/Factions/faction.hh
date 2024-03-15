#pragma once

#include <vector>

#include "special_ability.hh"
#include "ship.hh"

typedef int Amount;
typedef int Size;
typedef std::pair<Amount, Size> PossibleShip;
typedef std::vector<PossibleShip> PossibleShips;
typedef std::vector<SpecialAbility> SpecialAbilities;

class Faction {
  private:
    std::string _name;
    PossibleShips _possible_ships;
    SpecialAbilities _special_abilities;

  protected:
    virtual void setName(std::string name) { this->_name = name;};
    virtual void setPossibleShips(PossibleShips possible_ships) { this->_possible_ships = possible_ships;};
    virtual void addSpecialAbility(SpecialAbility new_ability) { this->_special_abilities.push_back(new_ability);};
    virtual void setSpecialAbilities(SpecialAbilities abilities) { this->_special_abilities = abilities;};

  public:
    Faction() = default;
    Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities): _name{name}, _possible_ships{possible_ships}, _special_abilities{special_abilities} {};

    // Getters
    virtual std::string getName() { return _name; };
    virtual PossibleShips getPossibleShips() { return _possible_ships; };
    virtual SpecialAbilities getSpecialAbilities() { return _special_abilities; };    
    virtual ~Faction() = default;
};