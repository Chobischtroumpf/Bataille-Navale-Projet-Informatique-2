#pragma once

#include <nlohmann/json.hpp>

#include "game.hh"
#include "player_role.hh"

class GameState {
  private:

    std::shared_ptr<Game> _game;

    enum Phase {
        NOT_STARTED=0,
        PLACING_SHIPS=1,
        PLAYING=2,
        FINISHED=3
    };

    Phase _phase = Phase::NOT_STARTED;
    bool handlePlaceShip(PlayerRole player, const nlohmann::json& move);

    bool handleFire(PlayerRole player, const nlohmann::json& move);

    bool handleChooseFaction(PlayerRole player, const nlohmann::json& move);

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

    // get the current game phase
    Phase getPhase() const;

    // translate game phase to bool
    bool hasStarted() const;

};
