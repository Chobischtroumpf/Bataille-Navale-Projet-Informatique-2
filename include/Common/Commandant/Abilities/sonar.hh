#pragma once

#include "special_ability.hh"

class Sonar : public virtual SpecialAbility
{
  public:
    Sonar(int cost=4) : SpecialAbility{"Sonar", "capacité permettant de poser des mines pendant la partie", cost, SONAR} {};
    ~Sonar() = default;
};