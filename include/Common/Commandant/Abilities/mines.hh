#pragma once

#include "special_ability.hh"

class Mine : public SpecialAbility
{
  public:
    Mine(int cost=2) : SpecialAbility{"Mine", "capacité permettant de poser des mines pendant la partie", cost, MINE} {};
    ~Mine() = default;
};