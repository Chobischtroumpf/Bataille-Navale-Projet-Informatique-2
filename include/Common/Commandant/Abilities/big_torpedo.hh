#pragma once

#include "special_ability.hh"

class BigTorpedo : public SpecialAbility
{
  public:
    BigTorpedo() : SpecialAbility{"Big Torpedo", "upgrade to the basic attack, targets an area of four cells", 4, BIG_TORPEDO} {};
    ~BigTorpedo() = default;
};