//#include "special_ability.hh"
#include "../../include/Common/Commandant/Abilities/special_ability.hh"

SpecialAbility::SpecialAbility(std::string name, std::string description, int energy_cost, SpecialAbilityType type) : _name{name}, _description{description}, _energy_cost{energy_cost}, _type{type} {}

bool SpecialAbility::operator==(const SpecialAbility &other) const { 
    return this->_name == other._name && this->_description == other._description && this->_energy_cost == other._energy_cost && this->_type == other._type;
}

void SpecialAbility::setName(std::string new_name) { this->_name = new_name; }
void SpecialAbility::setDescription(std::string new_description) { this->_description = new_description; }
void SpecialAbility::setEnergyCost(int new_cost) { this->_energy_cost = new_cost; }
void SpecialAbility::setType(SpecialAbilityType new_type) { this->_type = new_type; }

std::string SpecialAbility::getName() const { return _name; };
std::string SpecialAbility::getDescription() const { return _description; };
int SpecialAbility::getEnergyCost() const { return _energy_cost; };
SpecialAbilityType SpecialAbility::getType() const { return _type; };