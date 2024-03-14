#pragma once

#include <string>

typedef enum
{
    NONE =          0b000000,
    IS_LINE =       0b001000,
    IS_AREA =       0b010000,
    IS_MINE =       0b000001,
    IS_SONAR =      0b000010,
    IS_TORPEDO =    0b000100,
    IS_PERIMETER =  0b100000,
    
    MINE = IS_MINE,
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
    std::string name_;
    std::string description_;
    int energy_cost_;
    SpecialAbilityType type_;
  protected:
    void    setName(std::string new_name) { this->name_ = new_name; };
    void    setDescription(std::string new_description) { this->description_ = new_description; };
    void    setEnergyCost(int new_cost) { this->energy_cost_ = new_cost; };
    void    setType(SpecialAbilityType new_type) { this->type_ = new_type; };
  public:
    SpecialAbility() = delete;
    SpecialAbility(std::string name, std::string description="", int energy_cost=0, SpecialAbilityType type=NONE) : name_{name}, description_{description}, energy_cost_{energy_cost}, type_{type} {};
    std::string getName() { return name_; };
    std::string getDescription() { return description_; };
    int     getEnergyCost() { return energy_cost_; };
    SpecialAbilityType getType() { return type_; };

    virtual ~SpecialAbility() = default;
};