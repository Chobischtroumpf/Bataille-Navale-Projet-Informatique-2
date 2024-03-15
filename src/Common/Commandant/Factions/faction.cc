#include "faction.hh"

Faction::Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities): _name{name}, _possible_ships{possible_ships}, _special_abilities{special_abilities} {}

void Faction::setName(std::string name) { this->_name = name;}
void Faction::setPossibleShips(PossibleShips possible_ships) { this->_possible_ships = possible_ships;}
void Faction::addSpecialAbility(SpecialAbility new_ability) { this->_special_abilities.push_back(new_ability);}
void Faction::setSpecialAbilities(SpecialAbilities abilities) { this->_special_abilities = abilities;}

std::string Faction::getName() const { return _name; };
PossibleShips Faction::getPossibleShips() const { return _possible_ships; };
SpecialAbilities Faction::getSpecialAbilities() const { return _special_abilities; };