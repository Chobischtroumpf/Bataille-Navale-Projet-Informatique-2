#pragma once

#include <vector>
#include <map>

#include "ship.hh"
#include "special_ability.hh"

typedef int Amount; // Amount of ships
typedef int Size; // Size of ships
typedef std::pair<Size, Amount> PossibleShip; // defines a type that is a pair containing the amount and size of a ship a faction can have
typedef std::map<Size, Amount> PossibleShips; // defines a type that is a map that contains all the PossibleShip's a faction can have
typedef std::vector<SpecialAbility> SpecialAbilities; // defines a type that contains all the special abilities a faction has

class Faction {
  private:
    std::string _name; // Name of the faction
    PossibleShips _possible_ships; // Possible ships the faction can have
    SpecialAbilities _special_abilities;  // Special abilities the faction has

  protected:
    // Protected setters for the derived classes
    virtual void setName(std::string name); // Set the name of the faction
    virtual void setPossibleShips(PossibleShips possible_ships); // Set the possible ships the faction can have
    virtual void addSpecialAbility(SpecialAbility new_ability); // Add a special ability to the faction
    virtual void setSpecialAbilities(SpecialAbilities abilities); // Set the special abilities the faction has

  public:
    // Constructors
    Faction() = default;
    Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities); 

    // Operators
    bool operator==(const Faction &other) const;

    // Getters
    virtual std::string getName() const;
    virtual PossibleShips getPossibleShips() const;
    virtual SpecialAbilities getSpecialAbilities() const;

    virtual Amount getAmountOfShips() const;
    
    // Destructor
    virtual ~Faction() = default;
};