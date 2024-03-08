#pragma once

#include "special_ability.hh"

class Torpedo : public SpecialAbility
{
  public:
    Torpedo() : SpecialAbility{"Torpedo", "basic attack, targets a single cell", 0, TORPEDO} {};
    ~Torpedo() = default;
};