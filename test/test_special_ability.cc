#include <iostream>
#include <cassert>
#include "special_ability.hh"


class TestSpecialAbility : public SpecialAbility
{
  public:
    TestSpecialAbility(std::string name, std::string description, int energy_cost, SpecialAbilityType type) : SpecialAbility(name, description, energy_cost, type) {}
    void    setName(std::string new_name) { SpecialAbility::setName(new_name); }
    void    setDescription(std::string new_description) { SpecialAbility::setDescription(new_description); }
    void    setEnergyCost(int new_cost) { SpecialAbility::setEnergyCost(new_cost); }
    void    setType(SpecialAbilityType new_type) { SpecialAbility::setType(new_type); }
    std::string getName() { return SpecialAbility::getName(); }
    std::string getDescription() { return SpecialAbility::getDescription(); }
    int     getEnergyCost() { return SpecialAbility::getEnergyCost(); }
    SpecialAbilityType getType() { return SpecialAbility::getType(); }
};

void testSpecialAbility()
{
    // Create a SpecialAbility object
    TestSpecialAbility ability("Test Ability", "This is a test ability", 10, MINE);

    // Test the getters
    assert(ability.getName() == "Test Ability");
    assert(ability.getDescription() == "This is a test ability");
    assert(ability.getEnergyCost() == 10);
    assert(ability.getType() == MINE);

    // Test the setters
    ability.setName("Updated Ability");
    ability.setDescription("This is an updated test ability");
    ability.setEnergyCost(20);
    ability.setType(PIERCING_TORPEDO);

    assert(ability.getName() == "Updated Ability");
    assert(ability.getDescription() == "This is an updated test ability");
    assert(ability.getEnergyCost() == 20);
    assert(ability.getType() == PIERCING_TORPEDO);

    // Test the equality operator
    SpecialAbility ability2("Test Ability", "This is a test ability", 10, MINE);
    SpecialAbility ability3("Test Ability", "This is a test ability", 10, MINE);
    assert(ability3 == ability2);

    std::cout << "All tests passed!" << std::endl;
}

int main()
{
    testSpecialAbility();

    return 0;
}