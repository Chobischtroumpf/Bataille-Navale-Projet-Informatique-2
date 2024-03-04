#include "../../../include/server/game/game.hh"

Game::Game(unsigned int player1_id, unsigned int player2_id,
                       std::vector<unsigned int> &spectators,
                       bool mode_commandant, int time_game, int time_player)
    : _board{std::make_shared<Board>()}, _player1_id{player1_id},
      _player2_id{player2_id}, _spectators{spectators},
      _mode_commandant{mode_commandant}, _game_timer{time_game},
      _player_timer{time_player}, _is_timer_finished{false} {
        start_timer();
      }

bool Game::is_finished() const {
  return _board->isFinished() || _is_timer_finished;
}

void Game::start_timer() {
  _game_timer.start(std::bind(&Game::game_timer_finished, this));
  _player_timer.start(std::bind(&Game::player_timer_finished, this));
}

void Game::game_timer_finished() {
  _is_timer_finished = true;
  // Handle it by sending both players to disconnect
}

void Game::player_timer_finished() {
  _is_timer_finished = true;
  // Handle it by checking who s turn and setting the other as winner, might
  // use disconnect player
}

void Game::handle_place_ship(unsigned int player_id, ShipCoordinates ship_coordinates) {
  if (_board->myTurn() && player_id == _player1_id){  // if it s player ones turn and the id provided is player s one
    _board->placeShip(ship_coordinates, _board->myTurn());
    _board->changeTurn();
  } else if (!_board->myTurn() && player_id == _player2_id){
    _board->placeShip(ship_coordinates, !_board->myTurn());
    _board->changeTurn();
  } else{
    // handle if the it s not the turn of the id given or an unknown id
  }
  // (Inform the player that the boat was placed sucessfully or only send the json back ?)
}

void Game::handle_fire(unsigned int player_id, BoardCoordinates board_coordinates) {
  if (_board->myTurn() && player_id == _player1_id){
    _board->fire(board_coordinates);
    _board->changeTurn();
  } else if (!_board->myTurn() && player_id == _player2_id){
    _board->fire(board_coordinates);
    _board->changeTurn();
  }else{
    // handle if the it s not the turn of the id given or an unknown id
  }
}

nlohmann::json Game::get_state(unsigned int player_id){
  if (player_id == _player1_id ){
    return _board->toJson(PLAYERONE);
  }else if (player_id == _player2_id){
    return _board->toJson(PLAYERTWO);
  } else{
    return _board->toJsonSpectator();
  }
}
