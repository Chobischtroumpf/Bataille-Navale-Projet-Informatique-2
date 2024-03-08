#pragma once

#include "special_ability.hh"

class Mine : public SpecialAbility
{
  public:
    Mine() : SpecialAbility{"Mine", "capacité permettant de poser des mines pendant la partie", 2, MINE} {};
    ~Mine() = default;
};