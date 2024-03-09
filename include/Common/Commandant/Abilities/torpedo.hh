#pragma once

#include "special_ability.hh"

class Torpedo : public SpecialAbility
{
  public:
    Torpedo(int cost=4) : SpecialAbility{"Torpedo", "basic attack, targets a single cell", cost, TORPEDO} {};
    ~Torpedo() = default;
};