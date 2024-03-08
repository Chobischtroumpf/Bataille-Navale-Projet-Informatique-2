#pragma once

#include "special_ability.hh"

class Torpedo : public SpecialAbility
{
  public:
    Torpedo() : SpecialAbility{"Tir", "basic attack, aims for a single cell", 0, TORPEDO} {};
    ~Torpedo() = default;
};