#pragma once

#include <string>

typedef enum
{
    MINE,
    SONAR,
    PROBE,
    TORPEDO,
    AREA_DESTRUCTION,
    PERIMETER_DESTRUCTION,
}   SpecialAbilityType;

class SpecialAbility
{
  private:
    std::string name_;
    std::string description_;
    int energy_cost_;
    SpecialAbilityType type_;
    
  public:
    SpecialAbility(std::string name, std::string description, int energy_cost, SpecialAbilityType type) : name_{name}, description_{description}, energy_cost_{energy_cost}, type_{type} {};
    int     getEnergyCost() { return energy_cost_; };
    void    setEnergyCost(int new_cost) { this->energy_cost_ = new_cost; };
    SpecialAbilityType getType() { return type_; };

    virtual ~SpecialAbility() = default;
};