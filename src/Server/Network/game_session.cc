#include "game_session.hh"
#include "game_state.hh"

GameSession::GameSession(const std::string& leader_id, const nlohmann::json& game_details)
    : _game_details(game_details), _leader_id(leader_id), _game_state(game_details) {
    _participant_roles[leader_id] = PlayerRole::Leader;
    _session_name = game_details.at("name").get<std::string>();
    // hasStarted = false;
}

GameSession::~GameSession() {}

// void GameSession::startSession() {
//     this->hasStarted = true;
// }

void GameSession::endSession() {
    // Cleanup or end game logic
}

void GameSession::addParticipant(const std::string& participantId) {
    //Assign the role of the participant
    if (_opponent_id.empty()) {
        _opponent_id = participantId;
        _participant_roles[participantId] = PlayerRole::Opponent;
    } else {
        spectators.push_back(participantId);
        _participant_roles[participantId] = PlayerRole::Spectator;
    }
}

void GameSession::removeParticipant(const std::string& participantId) {
    // Remove participant from their respective list and role mapping
    _participant_roles.erase(participantId);
    if (participantId == _opponent_id) {
        // Handle reassignment or vacancy of Player B
        _opponent_id.clear();
    } else {
        spectators.erase(std::remove(spectators.begin(), spectators.end(), participantId), spectators.end());
    }
}

vector<std::string> GameSession::getParticipants() const {
    vector<std::string> participants;
    participants.emplace_back(_leader_id);

    if (!_opponent_id.empty()) {
        participants.emplace_back(_opponent_id);
    }

    for (const std::string& spectator : spectators) {
        participants.emplace_back(spectator);
    }

    return participants;
}

// Utility method to retrieve a participant role by id
PlayerRole GameSession::getParticipantRole(const std::string& participantId) const {
    auto it = _participant_roles.find(participantId);
    if (it != _participant_roles.end()) {
        return it->second;
    }
    return PlayerRole::Spectator; // Default or error handling
}

// Retrieves the game state
nlohmann::json GameSession::getGameState(const std::string& userId) const {
    auto playerRole= getParticipantRole(userId);

    return _game_state.getGameState(playerRole);
}

bool GameSession::makeMove(const std::string& userId, const nlohmann::json& move) {
    auto playerRole = getParticipantRole(userId);

    // Check if the move object contains "moveType" 
    std::clog << "Move: " << move << std::endl;
    if (!move.contains("moveType")) {
        // If "moveType" property is missing, return false indicating error
        std::clog << "Invalid move protocol: no move property on move object" << std::endl;
        return false;
    }
    return _game_state.makeMove(playerRole, move);
}

nlohmann::json GameSession::getSessionState() const {

    nlohmann::json sessionState;

    sessionState["participants"] = getParticipants();

    sessionState["hasStarted"] = _game_state.hasStarted();

    sessionState["sessionName"] = _session_name;
    
    return sessionState;
}