#include "game.hh"
#include "ship.hh"

#include <iostream>


int main() {
  Game game = Game(nlohmann::json::parse("{\"gamemode\": \"Classic\", \"turnTimeLimit\": 10, \"playerTimeLimit\": 10}"));

  Ship ship({0,0}, std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, true);
  std::clog << "Ship 1 created with top legt position : " << ship.getTopLeft() << std::endl;
  std::clog << "Ship 1 created with coordinates : " << std::endl;
  for (auto &c : ship.getCoordinates()) {
    std::clog << c;
  }
  std::clog << std::endl;
  Ship ship2({0,0}, std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, false);
  std::clog << "Ship 2 created with top legt position : " << ship2.getTopLeft() << std::endl;
  std::clog << "Ship 2 created with coordinates : " << std::endl;
  for (auto &c : ship2.getCoordinates()) {
    std::clog << c;
  }
  std::clog << std::endl;

  Ship ship3(BoardCoordinates(2,0), std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, true);
  std::clog << "Ship 3 created with top legt position : " << ship3.getTopLeft() << std::endl;
  std::clog << "Ship 3 created with coordinates : " << std::endl;
  for (auto &c : ship3.getCoordinates()) {
    std::clog << c;
  }
  std::clog << std::endl;
  
  Ship ship4({2,0}, std::vector<BoardCoordinates>{{0,0}, {0,1}, {1,1}}, false);
  std::clog << "Ship 4 created with top legt position : " << ship4.getTopLeft() << std::endl;
  std::clog << "Ship 4 created with coordinates : " << std::endl;
  for (auto &c : ship4.getCoordinates()) {
    std::clog << c;
  }
  std::clog << std::endl;

  std::clog << "Placing ships..." << std::endl;
  std::clog << "Placing ship 1" << std::endl;
  game.handlePlaceShip(PLAYERONE, ship, Phase::PLACING_SHIPS);
  std::clog << "Placing ship 2" << std::endl;
  game.handlePlaceShip(PLAYERTWO, ship2, Phase::PLACING_SHIPS);
  std::clog << "Placing ship 3" << std::endl;
  game.handlePlaceShip(PLAYERONE, ship3, Phase::PLACING_SHIPS);
  std::clog << "Placing ship 4" << std::endl;
  game.handlePlaceShip(PLAYERTWO, ship4, Phase::PLACING_SHIPS);

  std::clog << "Firing torpedos..." << std::endl;
  std::clog << "Firing torpedo 1" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {0,0});
  std::clog << "Firing torpedo 2" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {0,1});
  std::clog << "Firing torpedo 3" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {1,1});

  // auto p1 game._board->getPlayer1();
  // auto p2 game._board->getPlayer2();
  std::clog << "Firing torpedo 4" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {2,0});
  std::clog << "Firing torpedo 5" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {2,1});
  std::clog << "Firing torpedo 6" << std::endl;
  game.handleFire(PLAYERONE, TORPEDO, {3,1});
  // game.handleFire(PLAYERONE, TORPEDO, {1,1});

  nlohmann::json j = game.getState(PlayerRole::Spectator);
  // nlohmann::json j2 = game.getState(PlayerRole::Opponent);

  std::cout << j.dump(2) << std::endl;
  // std::cout << j2.dump(2) << std::endl;

}