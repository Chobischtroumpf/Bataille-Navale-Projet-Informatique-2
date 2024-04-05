#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "game_state.hh"

class GameSession {
  private:
    // Unique identifier for the session
    std::string _session_id;
    std::string _session_name;

    // // Bool indicating session state
    // bool hasStarted;

    // Game details (e.g., game type, rules)
    nlohmann::json _game_details;

    // Roles of the participants
    std::string _leader_id; // Player A
    std::string _opponent_id; // Player B (unassigned initially)
    std::vector<std::string> spectators;

    // Mapping from participant ID to their role
    std::unordered_map<std::string, PlayerRole> _participant_roles;

    // Instance of GameState to manage game logic
    GameState _game_state; 
 
  public:
    // Constructor with game leader userId and game details
    GameSession(const std::string& leader_id, const nlohmann::json& gameDetails);
    virtual ~GameSession();

    // // GameSession Management
    // void startSession();
    void endSession();

    // Session Participants Management
    void addParticipant(const std::string& participant_id);
    void removeParticipant(const std::string& participant_id);
    PlayerRole getParticipantRole(const std::string& participant_id) const;
    std::vector<std::string> getParticipants() const;

    // Game Logic 
    bool makeMove( const std::string& userId, const nlohmann::json& move );
    
    nlohmann::json getGameState(const std::string& userId) const;

    nlohmann::json getSessionState() const;
};
