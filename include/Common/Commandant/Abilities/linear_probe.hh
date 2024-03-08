#pragma once

#include "special_ability.hh"

class LinearProbe : public SpecialAbility
{
  public:
    LinearProbe() : SpecialAbility{"Tir", "capacité de base, vise une case simple", 0, TORPEDO} {};
    ~LinearProbe() = default;
};