#pragma once

#include "board.hh"
#include "../../../include/common/ship_coordinates.hh"
#include "timer.hh"
#include "../../../include/common/turn.hh"
#include "player_role.hh"

// Class that handles the game between 2 players
class Game {
public:
  Game(const nlohmann::json& game_details);

  bool is_finished() const;

  void game_timer_finished();

  void player_timer_finished();

  bool handle_place_ship(Turn turn, ShipCoordinates ship_coordinates);

  bool handle_fire(Turn turn, BoardCoordinates board_coordinates);

  nlohmann::json get_state(PlayerRole player);

private:
  std::shared_ptr<Board> _board;
  bool mode_commandant;
  Timer game_timer;
  Timer player_timer;
  bool is_timer_finished;

  void start_timer();
  
  void set_game(const nlohmann::json& gameDetails);
};