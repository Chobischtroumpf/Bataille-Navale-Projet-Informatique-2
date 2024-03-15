#pragma once

#include <string>

typedef enum
{
    NONE =          0b000000,
    MINE =          0b000001,
    IS_SONAR =      0b000010,
    IS_TORPEDO =    0b000100,
    IS_LINE =       0b001000,
    IS_AREA =       0b010000,
    IS_PERIMETER =  0b100000,
    
    SONAR = IS_SONAR | IS_AREA,
    PROBE = IS_SONAR | IS_LINE,
    TORPEDO = IS_TORPEDO,
    PIERCING_TORPEDO = IS_TORPEDO | IS_LINE,
    BIG_TORPEDO = IS_TORPEDO | IS_AREA,
    AERIAL_STRIKE = IS_TORPEDO | IS_PERIMETER
}   SpecialAbilityType;

class SpecialAbility
{
  private:
    std::string _name;
    std::string _description;
    int _energy_cost;
    SpecialAbilityType _type;
  protected:
    void    setName(std::string new_name);
    void    setDescription(std::string new_description);
    void    setEnergyCost(int new_cost);
    void    setType(SpecialAbilityType new_type);
  public:

    // Constructors
    SpecialAbility() = delete;
    SpecialAbility(std::string name, std::string description="", int energy_cost=0, SpecialAbilityType type=NONE);
    bool operator==(const SpecialAbility &other) const;
    std::string getName();
    std::string getDescription();
    int     getEnergyCost();
    SpecialAbilityType getType();

    virtual ~SpecialAbility() = default;
};