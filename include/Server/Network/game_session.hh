#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "game_state.hh"

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
    std::vector<std::string> getParticipants() const;

    // Game Logic 
    bool makeMove( const std::string& userId, const std::string& move );
    
    nlohmann::json getGameState(const std::string& userId) const;

    nlohmann::json getSessionState() const;

private:
    // Unique identifier for the session
    std::string sessionId;

    // Bool indicating session state
    bool hasStarted;

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
