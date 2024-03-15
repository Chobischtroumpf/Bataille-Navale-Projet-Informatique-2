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
    std::string _name;
    PossibleShips _possible_ships;
    SpecialAbilities _special_abilities;

  protected:
    virtual void setName(std::string name);
    virtual void setPossibleShips(PossibleShips possible_ships);
    virtual void addSpecialAbility(SpecialAbility new_ability);
    virtual void setSpecialAbilities(SpecialAbilities abilities);

  public:
    Faction() = default;
    Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities);

    // Operators
    bool operator==(const Faction &other) const;

    virtual std::string getName() const;
    virtual PossibleShips getPossibleShips() const;
    virtual SpecialAbilities getSpecialAbilities() const;
    virtual ~Faction() = default;

    virtual void removeShip(Amount amount);
};