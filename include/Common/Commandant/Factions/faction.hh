#pragma once

#include <vector>

#include "special_ability.hh"
#include "ship.hh"

typedef int Amount;
typedef int Size;
typedef std::pair<Amount, Size> PossibleShip;
typedef std::vector<PossibleShip> PossibleShips;
typedef std::vector<SpecialAbility> SpecialAbilities;
class Faction {
  private:
    std::string name_;
    PossibleShips ships_;
    SpecialAbilities special_abilities_;

  protected:
    virtual void setName(std::string name) { this->name_ = name;};
    virtual void setPossibleShips(PossibleShips ships) { this->ships_ = ships;};
    virtual void addSpecialAbility(SpecialAbility new_ability) { this->special_abilities_.push_back(new_ability);};
    virtual void setSpecialAbilities(std::vector<SpecialAbility> abilities) { this->special_abilities_ = abilities;};

  public:
    Faction() = default;
    Faction(std::string name, std::vector<PossibleShip> ships, std::vector<SpecialAbility> special_abilities): name_{name}, ships_{ships}, special_abilities_{special_abilities} {};

    virtual std::string getName() { return name_; };
    virtual std::vector<PossibleShip> getShips() { return ships_; };
    virtual std::vector<SpecialAbility> getSpecialAbilities() { return special_abilities_; };
    virtual ~Faction() = default;
};