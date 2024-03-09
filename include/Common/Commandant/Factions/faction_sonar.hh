#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "sonar.hh"
#include "probe.hh"
#include "torpedo.hh"
#include "piercing_torpedo.hh"
#include "aerial_strike.hh"


class FactionSonar : public Faction
{
  public:
    FactionSonar(): Faction() {
      setName("Sonar");
      PossibleShips ships;
      ships.push_back(std::make_pair(1, 1));
      ships.push_back(std::make_pair(3, 2));
      ships.push_back(std::make_pair(4, 3));
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo(), PiercingTorpedo(), Sonar(3), AerialStrike(), Probe()});
    };
    ~FactionSonar();
};

