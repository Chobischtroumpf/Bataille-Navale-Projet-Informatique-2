#pragma once

#include "board.hh"
#include "../../../include/common/ship_coordinates.hh"
#include "timer.hh"
#include "../../../include/common/turn.hh"

// Class that handles the game between 2 players and a list of spectators
class GameServer {
public:
  GameServer(unsigned int player1_id, unsigned int player2_id,
             std::vector<unsigned int> &spectators, bool mode_commandant,
             int time_game, int time_player);

  bool is_finished() const;

  void game_timer_finished();

  void player_timer_finished();

  void handle_place_ship(unsigned int player_id, ShipCoordinates ship_coordinates);

  void handle_fire(unsigned int player_id, BoardCoordinates board_coordinates);

  nlohmann::json get_state(unsigned int player_id);

private:
  std::shared_ptr<Board> _board;
  unsigned int _player1_id;
  unsigned int _player2_id;
  std::vector<unsigned int> _spectators;
  bool _mode_commandant;
  Timer _game_timer;
  Timer _player_timer;
  bool _is_timer_finished;

  void start_timer();
};
