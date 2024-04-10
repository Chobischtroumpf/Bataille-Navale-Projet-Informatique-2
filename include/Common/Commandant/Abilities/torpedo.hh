#pragma once

#include "special_ability.hh"

class Torpedo : public virtual SpecialAbility
{
  public:
    Torpedo(int cost=0) : SpecialAbility{"Torpedo", "basic attack, targets a single cell", cost, TORPEDO} {};
    ~Torpedo() = default;
};