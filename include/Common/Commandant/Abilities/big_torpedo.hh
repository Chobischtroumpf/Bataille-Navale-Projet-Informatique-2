#pragma once

#include "special_ability.hh"

class BigTorpedo : public SpecialAbility
{
  public:
    BigTorpedo(int cost=4) : SpecialAbility{"Big Torpedo", "upgrade OF the basic attack, targets an area of four cells", cost, BIG_TORPEDO} {};
    ~BigTorpedo() = default;
};