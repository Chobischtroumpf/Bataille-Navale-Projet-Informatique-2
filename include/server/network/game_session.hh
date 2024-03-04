#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "GameState.hpp"

class GameSession {
public:

    // Constructor with game leader userId and game details
    GameSession(const std::string& leaderId, const nlohmann::json& gameDetails);
    virtual ~GameSession();

    // GameSession Management
    void startSession();
    void endSession();

    // Session Participants Management
    void addParticipant(const std::string& participantId);
    void removeParticipant(const std::string& participantId);
    PlayerRole getParticipantRole(const std::string& participantId) const;

    // Game Logic 
    bool makeMove( const std::string& userId, const std::string& move );
    
    nlohmann::json getGameState(const std::string& userId) const;

private:
    // Unique identifier for the session
    std::string sessionId;

    // Game details (e.g., game type, rules)
    nlohmann::json gameDetails;

    // Roles of the participants
    std::string leaderId; // Player A
    std::string opponentId; // Player B (unassigned initially)
    std::vector<std::string> spectators;

    // Mapping from participant ID to their role
    std::unordered_map<std::string, PlayerRole> participantRoles;

    // Instance of GameState to manage game logic
    GameState gameState; 
};

#endif // GAMESESSION_H
