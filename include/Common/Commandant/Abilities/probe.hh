#pragma once

#include "special_ability.hh"

class Probe : public SpecialAbility
{
  public:
    Probe(int cost=4) : SpecialAbility{"Tir", "capacité de base, vise une case simple", cost, PROBE} {};
    ~Probe() = default;
};