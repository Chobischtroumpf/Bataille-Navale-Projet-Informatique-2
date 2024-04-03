#pragma once

#include "faction.hh"
#include "torpedo.hh"

class FactionClassique : public Faction
{
  public:
    // Constructor
    FactionClassique(): Faction() { // creates a Faction with preset values for the Classic faction
      setName("Classic");
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

