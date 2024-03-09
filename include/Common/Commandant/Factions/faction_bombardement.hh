#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "sonar.hh"
#include "probe.hh"
#include "torpedo.hh"
#include "piercing_torpedo.hh"
#include "aerial_strike.hh"

class FactionBombardement : public Faction
{
  public:
    FactionBombardement(): Faction() {
      setName("Sonar");
      PossibleShips ships;
      ships.push_back(std::make_pair(1, 1));
      ships.push_back(std::make_pair(1, 2));
      ships.push_back(std::make_pair(1, 3));
      ships.push_back(std::make_pair(2, 4));
      ships.push_back(std::make_pair(1, 5));
      setPossibleShips(ships);
      setSpecialAbilities({Torpedo(), PiercingTorpedo(), Sonar(3), AerialStrike(), Probe()});
    };
    ~FactionBombardement() {};
};

