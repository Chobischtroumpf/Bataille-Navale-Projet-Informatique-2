#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "torpedo.hh"

class FactionClassique : public Faction
{
  public:
    FactionClassique(): Faction() {
      setName("Sonar");
      PossibleShips ships;
      ships.push_back(std::make_pair(1, 2));
      ships.push_back(std::make_pair(2, 3));
      ships.push_back(std::make_pair(1, 4));
      ships.push_back(std::make_pair(1, 5));
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo()});
    };
    ~FactionClassique() {};
};

