#pragma once

#include "faction.hh"
#include "special_ability.hh"

class FactionSonar : public Faction
{
  public:
    FactionSonar(std::string name, std::vector<Ship> ships, std::vector<SpecialAbility> special_abilities): Faction(name, ships, special_abilities) {};
    ~FactionSonar();
};

