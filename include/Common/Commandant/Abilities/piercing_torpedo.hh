#pragma once

#include "special_ability.hh"

class PiercingTorpedo : public virtual SpecialAbility
{
  public:
    PiercingTorpedo(int cost = 4) : SpecialAbility{"Piercing Torpedo", "upgrade of the basic attack, targets a line of four cells", cost, PIERCING_TORPEDO} {};
    ~PiercingTorpedo() = default;
};