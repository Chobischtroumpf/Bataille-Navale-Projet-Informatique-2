#pragma once

#include "ship.hh"
#include "turn.hh"
#include "board.hh"
#include "game_timer.hh"
#include "player_role.hh"



// Class that handles the game between 2 players
class Game {
  private:
    std::shared_ptr<Board> _board;
    bool _game_started;
    bool _mode_commandant;
    GameTimer _game_timer;
    bool _update_player1;
    bool _update_player2;

    void startTimer();
    
    void setGame(const nlohmann::json& gameDetails);

    bool shipPlacementsFinished() const;

    void changeTurn();

  public:
    Game(const nlohmann::json& game_details);

    bool isFinished() const;

    bool handlePlaceShip(Turn turn, Ship ship);

    bool handleFire(Turn turn, SpecialAbilityType ability_type, BoardCoordinates board_coordinates);

    nlohmann::json getState(PlayerRole player);
};