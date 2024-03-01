#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <nlohmann/json.hpp>
#include <string>

enum class PlayerRole {
        Leader, // Player A
        Opponent, // Player B
        Spectator // Not directly interacting, but may need game state info
};

class GameState {
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
    bool makeMove(PlayerRole player, const std::string& move);

    // Get the current state of the game
    nlohmann::json getGameState(PlayerRole player) const;

private:
    // Game details and rules
    nlohmann::json gameDetails;

    // Current state of the game
    nlohmann::json currentState;

    // Internal methods for handling game logic
    void updateState();
};

#endif // GAMESTATE_H
