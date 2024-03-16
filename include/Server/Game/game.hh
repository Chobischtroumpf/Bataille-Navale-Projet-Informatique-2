#pragma once

#include <vector>

#include "turn.hh"
#include "board.hh"
#include "game_timer.hh"
#include "player_role.hh"
//#include "ship_coordinates.hh"
#include "ship.hh"
// #include "ship_coordinates.hh"

// Class that handles the game between 2 players
class Game {
public:
  Game(const nlohmann::json& game_details);

  bool is_finished() const;

  bool handle_place_ship(Turn turn, Ship ship);

  bool handle_fire(Turn turn, BoardCoordinates board_coordinates);

  nlohmann::json get_state(PlayerRole player);

private:
  const size_t required_ship_placements = 5; // Number of ship placements required
  std::shared_ptr<Board> _board;
  bool mode_commandant;
  GameTimer game_timer;
  std::unordered_map<Turn, unsigned int> ship_placements;
  bool update_player1;
  bool update_player2;

  void start_timer();
  
  void set_game(const nlohmann::json& gameDetails);

  void initialize_ship_placements();

  bool ship_placements_finished() const;

  void change_turn();
};