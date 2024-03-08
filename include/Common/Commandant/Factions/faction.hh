#pragma once

#include <vector>

#include "special_ability.hh"
#include "ship.hh"

class Faction {
  protected:
    std::string name_;
    std::vector<Ship> ships_;
    std::vector<SpecialAbility> special_abilities_;

  public:
    Faction(std::string name, std::vector<Ship> ships, std::vector<SpecialAbility> special_abilities): name_{name}, ships_{ships}, special_abilities_{special_abilities} {};
    virtual std::string getName() { return name_; };
    virtual std::vector<Ship> getShips() { return ships_; };
    virtual std::vector<SpecialAbility> getSpecialAbilities() { return special_abilities_; };
    virtual ~Faction() = default;
};