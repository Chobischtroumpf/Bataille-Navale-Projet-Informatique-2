//#include "faction.hh"
#include "../../include/Common/Commandant/Factions/faction.hh"

Faction::Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities): _name{name}, _possible_ships{possible_ships}, _special_abilities{special_abilities} {}

bool Faction::operator==(const Faction& other) const { 
    return _name == other._name && _possible_ships == other._possible_ships && _special_abilities == other._special_abilities;
}

void Faction::setName(std::string name) { this->_name = name;}
void Faction::setPossibleShips(PossibleShips possible_ships) { this->_possible_ships = possible_ships;}
void Faction::addSpecialAbility(SpecialAbility new_ability) { this->_special_abilities.push_back(new_ability);}
void Faction::setSpecialAbilities(SpecialAbilities abilities) { this->_special_abilities = abilities;}

std::string Faction::getName() const { return _name; };
PossibleShips Faction::getPossibleShips() const { return _possible_ships; };
SpecialAbilities Faction::getSpecialAbilities() const { return _special_abilities; };

Amount Faction::getAmountOfShips() const {
    Amount amount = 0;
    for (auto& ship : _possible_ships) {
        amount += ship.first;
    }
    return amount;
}