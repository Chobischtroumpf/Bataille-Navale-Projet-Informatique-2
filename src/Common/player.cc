#include "player.hh"

Player::Player(Faction faction): _faction{faction}, _is_turn{false}, _energy_points{0} {}

Faction Player::getFaction() const {
  return _faction;
}

std::vector<Ship> Player::getFleet() const {
  return _fleet;
}

bool Player::isTurn() const {
  return _is_turn;
}

int Player::getEnergyPoints() const {
  return _energy_points;
}

void Player::setFaction(Faction faction) {
  _faction = faction;
}

void Player::setFleet(std::vector<Ship> fleet) {
  _fleet = fleet;
}

void Player::addShip(Ship ship) {
  _fleet.push_back(ship);
}

void Player::setTurn(bool is_turn) {
  _is_turn = is_turn;
}

void Player::setEnergyPoints(int energy_points) {
  _energy_points = energy_points;
}