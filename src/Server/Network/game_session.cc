#include "game_session.hh"
#include "game_state.hh"

GameSession::GameSession(const std::string& leaderId, const nlohmann::json& gameDetails)
    : leaderId(leaderId), gameDetails(gameDetails), gameState(gameDetails) {
    participantRoles[leaderId] = PlayerRole::Leader;
}

GameSession::~GameSession() {}

void GameSession::startSession() {
    // Initialize or start game logic - TBA
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

// Method that sends a move to the game state
bool GameSession::makeMove(const std::string& userId, const std::string& move) {
    auto playerRole = getParticipantRole(userId);

    return gameState.makeMove(playerRole, move);
}