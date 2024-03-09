#pragma once

#include "special_ability.hh"

class AerialStrike : public SpecialAbility
{
  public:
    AerialStrike(int cost=9) : SpecialAbility{"Aerial Strike", "an aerial strike that targets the cells in a 4x4 perimeter", cost, AERIAL_STRIKE} {};
    ~AerialStrike() = default;
};