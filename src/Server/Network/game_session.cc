#include "game_session.hh"
#include "game_state.hh"

GameSession::GameSession(std::string& sessionId, Queries& dbManager, const std::string& leaderId, const nlohmann::json& gameDetails)
    : _session_id(sessionId), dbManager(dbManager), _leader_id(leaderId), _game_details(gameDetails), _game_state(gameDetails) {
    _participant_roles[leaderId] = PlayerRole::Leader;
    hasStarted = false;
    _session_name = _game_details.at("name").get<std::string>();
    std::clog << "GameSession created: " << _session_name << std::endl;
}

GameSession::~GameSession() {}

void GameSession::startSession() {
    this->hasStarted = true;
}

void GameSession::endSession() {
    // Cleanup or end game logic
    std::cout << "Saving game history: " << _session_id << std::endl << _game_history.dump() << std::endl;
    
}

bool GameSession::addParticipant(const std::string& participantId) {
    std::lock_guard<std::mutex> guard(sessionMutex);

    //Assign the role of the participant
    if (_opponent_id.empty()) {
        _opponent_id = participantId;
        _participant_roles[participantId] = PlayerRole::Opponent;
    } else {
        if ( (int)spectators.size() >= (int)_game_details.at("maxPlayers") - 2 || spectators.size() >= 6 ) {
            std::cout << "Reached player limit in game session " << _session_id << " ... joining game not allowed" << std::endl;
            return false;
        }
        spectators.push_back(participantId);
        _participant_roles[participantId] = PlayerRole::Spectator;
    }
    return true;
}

void GameSession::removeParticipant(const std::string& participantId) {
    std::lock_guard<std::mutex> guard(sessionMutex);
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
    std::lock_guard<std::mutex> guard(sessionMutex);
    auto playerRole= getParticipantRole(userId);

    return _game_state.getGameState(playerRole);
}

bool GameSession::makeMove(const std::string& userId, const nlohmann::json& move) {
    std::lock_guard<std::mutex> guard(sessionMutex);
    auto playerRole = getParticipantRole(userId);

    // Check if the move object contains "moveType" 
    std::clog << "Move: " << move << std::endl;
    if (!move.contains("moveType")) {
        // If "moveType" property is missing, return false indicating error
        std::clog << "Invalid move protocol: no move property on move object" << std::endl;
        return false;
    }

    std::string moveType = move["moveType"];
    // Check if the moveType is "StartGame" or "EndGame"
    if (moveType == "StartGame") {
        // If moveType is "StartGame" or "EndGame", update the hasStarted property accordingly
        hasStarted = true;
    } else if ( moveType == "EndGame") {
        endSession();
        return true;
        // Nothing yet
    }
    // Call makeMove on the gameState and return the result
    bool result = _game_state.makeMove(playerRole, move);

    // Update the state history if the move was successful
    if (result) { updateHistory(); }

    return result;
}

nlohmann::json GameSession::getSessionState() const {

    nlohmann::json sessionState;

    sessionState["participants"] = getParticipants();

    sessionState["hasStarted"] = _game_state.hasStarted();

    std::clog << "Session State: " << _session_name << std::endl;

    sessionState["sessionName"] = _session_name;
    
    return sessionState;
}

// Returns the game's history
nlohmann::json GameSession::getHistory() const {
    return _game_history;
}

// Updates the game history by appending the current game state
void GameSession::updateHistory() {
    // Get the current game state
    nlohmann::json state = _game_state.getGameState(PlayerRole::Spectator);

    // Append the current game state to the game history
    _game_history["states"].push_back(state);

    if ( !dbManager.addGameState(_leader_id, _opponent_id, _session_id, state.dump())) {
                std::cerr << "Error saving game history" << std::endl;
    }
    
}