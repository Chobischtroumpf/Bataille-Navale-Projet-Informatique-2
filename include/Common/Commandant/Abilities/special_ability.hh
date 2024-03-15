#pragma once

#include <string>

typedef enum
{
    NONE =          0b000000,
    _IS_LINE =       0b001000,
    _IS_AREA =       0b010000,
    _IS_MINE =       0b000010,
    _IS_SONAR =      0b000001,
    _IS_TORPEDO =    0b000100,
    _IS_PERIMETER =  0b100000,
    
    MINE = _IS_MINE,
    SONAR = _IS_SONAR | _IS_AREA,
    PROBE = _IS_SONAR | _IS_LINE,
    TORPEDO = _IS_TORPEDO,
    PIERCING_TORPEDO = _IS_TORPEDO | _IS_LINE,
    BIG_TORPEDO = _IS_TORPEDO | _IS_AREA,
    AERIAL_STRIKE = _IS_TORPEDO | _IS_PERIMETER
}   SpecialAbilityType;

class SpecialAbility
{
  private:
    std::string name_;
    std::string description_;
    int energy_cost_;
    SpecialAbilityType type_;
  protected:
    void    setName(std::string new_name);
    void    setDescription(std::string new_description);
    void    setEnergyCost(int new_cost);
    void    setType(SpecialAbilityType new_type);
  public:
    SpecialAbility() = delete;
    SpecialAbility(std::string name, std::string description="", int energy_cost=0, SpecialAbilityType type=NONE);
    std::string getName();
    std::string getDescription();
    int     getEnergyCost();
    SpecialAbilityType getType();

    virtual ~SpecialAbility() = default;
};