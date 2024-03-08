#pragma once

#include "special_ability.hh"

class Sonar : public SpecialAbility
{
  public:
    Sonar() : SpecialAbility{"Sonar", "capacité permettant de poser des mines pendant la partie", 4, SONAR} {};
    ~Sonar() = default;
};