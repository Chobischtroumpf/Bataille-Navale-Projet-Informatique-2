#include "session_manager.hh"

using njson = nlohmann::json;

SessionManager& SessionManager::getInstance() {
    static SessionManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

SessionManager::SessionManager() {
    // Constructor body, if needed
}

string SessionManager::createSession(const string& userId, const njson& gameDetails, Queries& dbManager) {
    lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    // Generate a unique session ID
    stringstream ss;
    random_device rd;
    for (int i = 0; i < 8; ++i) { // Generate a random 8-character hex string
        ss << hex << rd();
    }

    string sessionId = ss.str();

    // Create and store the new session
    sessions[sessionId] = make_shared<GameSession>(sessionId, dbManager, userId, gameDetails);

    return sessionId;
}
bool SessionManager::sessionExists(const string& sessionId) {
    auto it = sessions.find(sessionId);
    return it != sessions.end();
}

shared_ptr<GameSession> SessionManager::getSession(const string& sessionId) {
    lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        return it->second;
    } else {
        // Return null if the session is not found
        return nullptr;
    }
}

void SessionManager::endSession(const string& sessionId) {
    lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        // Properly end the session
        it->second->endSession(); 
        sessions.erase(it);
    }
}
