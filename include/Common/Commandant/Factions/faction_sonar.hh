#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "sonar.hh"
#include "probe.hh"
#include "torpedo.hh"
#include "piercing_torpedo.hh"
#include "aerial_strike.hh"


class FactionSonar : public virtual Faction
{
  public:
    // Constructor
    FactionSonar(): Faction() { // creates a Faction with preset values for the Sonar faction
      setName("Sonar");
      PossibleShips ships;
      ships[1]=1;
      ships[2]=3;
      ships[3]=4;
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo(), PiercingTorpedo(), Sonar(3), AerialStrike(), Probe()});
    };
    ~FactionSonar() {};
};

