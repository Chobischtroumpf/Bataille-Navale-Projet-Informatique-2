#pragma once

#include "faction.hh"
#include "special_ability.hh"
#include "mines.hh"
#include "sonar.hh"
#include "torpedo.hh"
#include "piercing_torpedo.hh"
#include "aerial_strike.hh"

class FactionMines : public Faction
{
  public:
    // Constructor
    FactionMines(): Faction() { // creates a Faction with preset values for the Mines faction
        setName("Sonar");
        PossibleShips ships;
        ships[1] = 1;
        ships[2] = 2;
        ships[3] = 2;
        ships[4] = 1;
        setPossibleShips(ships);
        setSpecialAbilities({Torpedo(), PiercingTorpedo(), Sonar(), AerialStrike(), Mine()});
    };
    ~FactionMines() {};
};

