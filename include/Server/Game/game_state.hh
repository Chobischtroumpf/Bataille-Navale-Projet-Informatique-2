#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <nlohmann/json.hpp>

#include "game.hh"
#include "player_role.hh"

class GameState {
  private:

    std::shared_ptr<Game> _game;
    /*// Game details and rules
    nlohmann::json gameDetails;

    // Current state of the game
    nlohmann::json currentState;*/
    bool _has_started;

    bool handlePlaceShip(PlayerRole player, const nlohmann::json& move);

    bool handleFire(PlayerRole player, const nlohmann::json& move);

    void handleLaunch();

    Turn role_to_turn(PlayerRole player);

  public:
    // Constructor with game details
    explicit GameState(const nlohmann::json& gameDetails);

    // Disable copy and move semantics
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(GameState&&) = delete;
    GameState& operator=(GameState&&) = delete;

    // Destructor
    virtual ~GameState();

    // Make a move in the game
    bool makeMove(PlayerRole player, const nlohmann::json& move );

    // Get the current state of the game
    nlohmann::json getGameState(PlayerRole player) const;


};

#endif // GAMESTATE_H
