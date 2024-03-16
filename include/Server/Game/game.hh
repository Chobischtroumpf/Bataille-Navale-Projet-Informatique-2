#pragma once

#include <vector>

#include "turn.hh"
#include "board.hh"
#include "game_timer.hh"
#include "player_role.hh"
#include "ship.hh"

// Class that handles the game between 2 players
class Game {
public:
  Game(const nlohmann::json& game_details);

  bool isFinished() const;

  bool handlePlaceShip(Turn turn, Ship ship);

  bool handleFire(Turn turn, BoardCoordinates board_coordinates);

  nlohmann::json getState(PlayerRole player);

private:
  const size_t _required__ship_placements = 5; // Number of ship placements required
  std::shared_ptr<Board> _board;
  bool _mode_commandant;
  GameTimer _game_timer;
  std::unordered_map<Turn, unsigned int> _ship_placements;
  bool _update_player1;
  bool _update_player2;

  void startTimer();
  
  void setGame(const nlohmann::json& gameDetails);

  void initializeShipPlacements();

  bool shipPlacementsFinished() const;

  void changeTurn();
};