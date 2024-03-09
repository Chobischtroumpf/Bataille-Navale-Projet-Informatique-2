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
    FactionMines(): Faction() {
        setName("Sonar");
        PossibleShips ships;
        ships.push_back(std::make_pair(1, 1));
        ships.push_back(std::make_pair(2, 2));
        ships.push_back(std::make_pair(2, 3));
        ships.push_back(std::make_pair(1, 4));
        setPossibleShips(ships);
        setSpecialAbilities({Torpedo(), PiercingTorpedo(), Sonar(), AerialStrike(), Mine()});
    };
    ~FactionMines();
};

