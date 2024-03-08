#pragma once

#include "special_ability.hh"

class PiercingTorpedo : public SpecialAbility
{
  public:
    PiercingTorpedo() : SpecialAbility{"Piercing Torpedo", "upgrade to the basic attack, targets a line of four cells", 4, BIG_TORPEDO} {};
    ~PiercingTorpedo() = default;
};