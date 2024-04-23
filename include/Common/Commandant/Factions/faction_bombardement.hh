#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "sonar.hh"
#include "probe.hh"
#include "torpedo.hh"
#include "piercing_torpedo.hh"
#include "big_torpedo.hh"
#include "aerial_strike.hh"

class FactionBombardement : public virtual Faction
{
  public:
    // Constructor
    FactionBombardement(): Faction() { // creates a Faction with preset values for the Bombardement faction
      setName("Bombardement");
      PossibleShips ships;
      ships[1]=1;
      ships[2]=1;
      ships[3]=2;
      ships[4]=2;
      ships[5]=1;
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo(), PiercingTorpedo(), BigTorpedo(), Sonar(), AerialStrike(7)});
    };
    ~FactionBombardement() {};
};

