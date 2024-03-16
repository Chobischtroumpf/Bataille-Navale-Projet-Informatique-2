#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "torpedo.hh"

class FactionClassique : public Faction
{
  public:
    // Constructor
    FactionClassique(): Faction() { // creates a Faction with preset values for the Classic faction
      setName("Sonar");
      PossibleShips ships;
      ships[2]=1;
      ships[3]=2;
      ships[4]=1;
      ships[5]=1;
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo()});
    };
    ~FactionClassique() {};
};

