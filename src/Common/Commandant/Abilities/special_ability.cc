#include "special_ability.hh"

SpecialAbility::SpecialAbility(std::string name, std::string description, int energy_cost, SpecialAbilityType type) : name_{name}, description_{description}, energy_cost_{energy_cost}, type_{type} {}

void SpecialAbility::setName(std::string new_name) { this->name_ = new_name; };
void SpecialAbility::setDescription(std::string new_description) { this->description_ = new_description; };
void SpecialAbility::setEnergyCost(int new_cost) { this->energy_cost_ = new_cost; };
void SpecialAbility::setType(SpecialAbilityType new_type) { this->type_ = new_type; };

std::string SpecialAbility::getName() { return name_; };
std::string SpecialAbility::getDescription() { return description_; };
int SpecialAbility::getEnergyCost() { return energy_cost_; };
SpecialAbilityType SpecialAbility::getType() { return type_; };