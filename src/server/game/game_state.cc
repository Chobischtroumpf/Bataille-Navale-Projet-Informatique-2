// GameState.cpp
#include "GameState.hpp"

GameState::GameState(const nlohmann::json& gameDetails) : gameDetails(gameDetails) {
    // Initialize currentState with some default value or based on gameDetails
    currentState = nlohmann::json::object();
}

GameState::~GameState() {
    // Cleanup resources if needed
}

bool GameState::makeMove(PlayerRole player, const std::string& move) {
    // Placeholder implementation
    // For now, we just return true to indicate the move was "successful"
    return true;
}

nlohmann::json GameState::getGameState(PlayerRole player) const {
    // Return a placeholder or current game state
    // For simplicity, we return currentState directly
    return currentState;
}
