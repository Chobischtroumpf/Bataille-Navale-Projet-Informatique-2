#pragma once

#include "special_ability.hh"

class AerialStrike : public SpecialAbility
{
  public:
    AerialStrike() : SpecialAbility{"Aerial Strike", "an aerial strike that targets the cells in a 4x4 perimeter", 9, AERIAL_STRIKE} {};
    ~AerialStrike() = default;
};