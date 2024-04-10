#include "session_manager.hh"

using njson = nlohmann::json;

SessionManager& SessionManager::getInstance() {
    static SessionManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

SessionManager::SessionManager() {
    // Constructor body, if needed
}

string SessionManager::createSession(const string& userId, const njson& gameDetails) {
    std::lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    // Generate a unique session ID
    std::stringstream ss;
    std::random_device rd;
    for (int i = 0; i < 8; ++i) { // Generate a random 8-character hex string
        ss << std::hex << rd();
    }

    string session_id = ss.str();

    // Create and store the new session
    sessions[session_id] = std::make_shared<GameSession>(userId, gameDetails);

    return session_id;
}
bool SessionManager::sessionExists(const string& session_id) {
    auto it = sessions.find(session_id);
    return it != sessions.end();
}

shared_ptr<GameSession> SessionManager::getSession(const string& session_id) {
    std::lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    auto it = sessions.find(session_id);
    if (it != sessions.end()) {
        return it->second;
    } else {
        // Return null if the session is not found
        return nullptr;
    }
}

void SessionManager::endSession(const string& session_id) {
    std::lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    auto it = sessions.find(session_id);
    if (it != sessions.end()) {
        // Properly end the session
        it->second->endSession(); 
        sessions.erase(it);
    }
}
