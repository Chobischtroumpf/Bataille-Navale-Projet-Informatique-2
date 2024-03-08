#pragma once

#include "faction.hh"

class FactionClassique : public Faction
{
  public:
    FactionClassique(std::string name, std::vector<Ship> ships): Faction{name, ships, {}} {};
    ~FactionClassique();
};
