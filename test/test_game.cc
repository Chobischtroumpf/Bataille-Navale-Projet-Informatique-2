#include "game.hh"
#include "ship.hh"

#include <iostream>


int main() {
  Game game = Game(nlohmann::json::parse("{\"gamemode\": \"Classic\", \"turnTimeLimit\": 10, \"playerTimeLimit\": 10}"));

  Ship ship({0,0}, std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, true);
  Ship ship2({0,0}, std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, false);

  game.handlePlaceShip(PLAYERONE, ship);
  game.handlePlaceShip(PLAYERTWO, ship2);

  game.handleFire(PLAYERONE, TORPEDO, {0,0});
  game.handleFire(PLAYERONE, TORPEDO, {0,1});
  game.handleFire(PLAYERONE, TORPEDO, {1,1});

  // auto p1 game._board->getPlayer1();
  // auto p2 game._board->getPlayer2();

  nlohmann::json j = game.getState(PlayerRole::Leader);
  nlohmann::json j2 = game.getState(PlayerRole::Opponent);

  std::cout << j.dump(2) << std::endl;
  std::cout << j2.dump(2) << std::endl;

}