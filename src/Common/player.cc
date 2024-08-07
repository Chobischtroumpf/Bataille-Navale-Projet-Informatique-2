#include "player.hh"
#include <iostream>

Player::Player(): _faction{FactionClassique()}, _is_turn{false}, _energy_points{0} {}

Player::Player(Faction faction): _faction{faction}, _is_turn{false}, _energy_points{0} {}

Player::Player(const Player& other): _faction{other._faction}, _fleet{std::move(other._fleet)}, _is_turn{other._is_turn}, _energy_points{other._energy_points} {}

Player& Player::operator=(const Player& other) {
  if (this != &other) {
    _faction = other._faction;
    _fleet = std::move(other._fleet);
    _is_turn = other._is_turn;
    _energy_points = other._energy_points;
  }
  return *this;
}

Faction Player::getFaction() const {
  return _faction;
}

Fleet Player::getFleet() const {
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

void Player::setFleet(Fleet fleet) {
  _fleet = fleet;
}

void Player::addShip(std::shared_ptr<Ship> ship) {
  std::clog << "PLAYER::ADD_SHIP" << std::endl;
  _fleet.push_back(ship);
}

void Player::setTurn(bool is_turn) {
  _is_turn = is_turn;
}

void Player::swapTurn() {
  _is_turn = !_is_turn;
}

void Player::setEnergyPoints(int energy_points) {
  _energy_points = energy_points;
}

void Player::addEnergyPoints(int energy_points) {
  _energy_points += energy_points;
}

void Player::removeEnergyPoints(int energy_points) {
  _energy_points -= energy_points;
}

void Player::setPlayerOne(bool is_player_one) {
  _is_player_one = is_player_one;
}

bool Player::isPlayerOne() const {
  return _is_player_one;
}