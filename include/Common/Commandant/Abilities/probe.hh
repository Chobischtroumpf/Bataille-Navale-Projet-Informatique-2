#pragma once

#include "special_ability.hh"

class Probe : public SpecialAbility
{
  public:
    Probe() : SpecialAbility{"Tir", "capacité de base, vise une case simple", 0, PROBE} {};
    ~Probe() = default;
};