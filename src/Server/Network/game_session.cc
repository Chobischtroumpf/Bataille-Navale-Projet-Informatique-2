#include "game_session.hh"
#include "game_state.hh"

GameSession::GameSession(const std::string& leaderId, const nlohmann::json& gameDetails)
    : leaderId(leaderId), gameDetails(gameDetails), gameState(gameDetails) {
    participantRoles[leaderId] = PlayerRole::Leader;
    _session_name = gameDetails.at("name").get<std::string>();
    hasStarted = false;
}

GameSession::~GameSession() {}

void GameSession::startSession() {
    this->hasStarted = true;
}

void GameSession::endSession() {
    // Cleanup or end game logic
}

void GameSession::addParticipant(const std::string& participantId) {
    //Assign the role of the participant
    if (opponentId.empty()) {
        opponentId = participantId;
        participantRoles[participantId] = PlayerRole::Opponent;
    } else {
        spectators.push_back(participantId);
        participantRoles[participantId] = PlayerRole::Spectator;
    }
}

void GameSession::removeParticipant(const std::string& participantId) {
    // Remove participant from their respective list and role mapping
    participantRoles.erase(participantId);
    if (participantId == opponentId) {
        // Handle reassignment or vacancy of Player B
        opponentId.clear();
    } else {
        spectators.erase(std::remove(spectators.begin(), spectators.end(), participantId), spectators.end());
    }
}

vector<std::string> GameSession::getParticipants() const {
    vector<std::string> participants;
    participants.emplace_back(leaderId);

    if (!opponentId.empty()) {
        participants.emplace_back(opponentId);
    }

    for (const std::string& spectator : spectators) {
        participants.emplace_back(spectator);
    }

    return participants;
}

// Utility method to retrieve a participant role by id
PlayerRole GameSession::getParticipantRole(const std::string& participantId) const {
    auto it = participantRoles.find(participantId);
    if (it != participantRoles.end()) {
        return it->second;
    }
    return PlayerRole::Spectator; // Default or error handling
}

// Retrieves the game state
nlohmann::json GameSession::getGameState(const std::string& userId) const {
    auto playerRole= getParticipantRole(userId);

    return gameState.getGameState(playerRole);
}

bool GameSession::makeMove(const std::string& userId, const nlohmann::json& move) {
    auto playerRole = getParticipantRole(userId);

    // Check if the move object contains "moveType" 
    if (!move.contains("moveType")) {
        // If "moveType" property is missing, return false indicating error
        std::cerr << "Invalid move protocol: no move property on move object" << std::endl;
        return false;
    }

    std::string moveType = move.at("moveType");

    // Check if the moveType is "StartGame" or "EndGame"
    if (moveType == "StartGame") {
        // If moveType is "StartGame" or "EndGame", update the hasStarted property accordingly
        hasStarted = true;
        return true;
    } else if ( moveType == "EndGame") {
        return false;
        // Nothing yet
    }

    // Call makeMove on the gameState and return the result
    return gameState.makeMove(playerRole, move);
}

nlohmann::json GameSession::getSessionState() const {

    nlohmann::json sessionState;

    sessionState["participants"] = getParticipants();

    sessionState["hasStarted"] = this->hasStarted;

    sessionState["sessionName"] = _session_name;
    
    return sessionState;
}