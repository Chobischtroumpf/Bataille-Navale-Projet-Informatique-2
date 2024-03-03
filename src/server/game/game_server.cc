#include "../../../include/server/game/game_server.hh"
#include "../../../include/server/game/board_control.hh"

GameServer::GameServer(unsigned int player1_id, unsigned int player2_id,
                       std::vector<unsigned int> &spectators,
                       bool mode_commandant, int time_game, int time_player)
    : _board{std::make_shared<Board>()}, _player1_id{player1_id},
      _player2_id{player2_id}, _spectators{spectators},
      _mode_commandant{mode_commandant}, _game_timer{time_game},
      _player_timer{time_player}, _is_timer_finished{false} {}

bool GameServer::is_finished() const {
  return _board->isFinished() || _is_timer_finished;
}

void GameServer::start_timer() {
  _game_timer.start(std::bind(&GameServer::game_timer_finished, this));
  _player_timer.start(std::bind(&GameServer::player_timer_finished, this));
}

void GameServer::game_timer_finished() {
  _is_timer_finished = true;
  // Handle it by sending both players to disconnect
}

void GameServer::player_timer_finished() {
  _is_timer_finished = true;
  // Handle it by checking who s turn and setting the other as winner, might
  // use disconnect player
}

void GameServer::handle_place_ship(unsigned int player_id) {
  bool placed = false;
  while (!(placed || _is_timer_finished)) {

    // Check if the player gave a position
    ShipCoordinates ship_coordinates;

    // This part can be done in the controller of the client
    if (!(ship_coordinates.x() < _board->width() &&
          ship_coordinates.y() < _board->height())) {
      continue;
    }

    placed = _controller->placeShip(ship_coordinates); //
  }
  // Inform the player that the boat was placed sucessfully, and also the list
  // of spectators
}

void GameServer::handle_fire(unsigned int player_id) {
  bool fired = false;
  while (!(fired || _is_timer_finished)) {
    //// Check if the player gave a position for the boats
    BoardCoordinates board_coordinates;

    // This part can be done in the controller of the client
    if (!(board_coordinates.x() < _board->width() &&
          board_coordinates.y() < _board->height())) {
      continue;
    }

    fired = _controller->fire(board_coordinates); //
  }
  // Infom the player that the fire was succefful
  // But also gotta find a way to see if it touched the ocean or the boats
  // (and inform spectators)
}

void GameServer::place_ships() {
  for (int i = 0; i < 10; i++) {
    _player_timer.reset();
    if (_board->whoseTurn() == PLAYERONE) {
      handle_place_ship(_player1_id);
    } else {
      handle_place_ship(_player2_id);
    }
  }
}

void GameServer::play() {
  start_timer();
  place_ships();
  while (!is_finished()) {
    _player_timer.reset();
    if (_board->whoseTurn() == PLAYERONE) {
      handle_fire(_player1_id);
    } else {
      handle_fire(_player2_id);
    }
  }
}

unsigned int GameServer::winner() const {
  if (_board->isFinished()) {
    if (_board->isVictory()) { // suppose that player 1 won
      return _player1_id;
    } else {
      return _player2_id;
    }
  }
  return 0;
}