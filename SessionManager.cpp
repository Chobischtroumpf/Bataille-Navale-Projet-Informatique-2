#include "SessionManager.hpp"
#include <string>
#include <random>
#include <sstream>

SessionManager& SessionManager::getInstance() {
    static SessionManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

SessionManager::SessionManager() {
    // Constructor body, if needed
}

string SessionManager::createSession() {
    lock_guard<mutex> guard(sessionsMutex); // Lock for thread safety

    // Generate a unique session ID
    stringstream ss;
    random_device rd;
    for (int i = 0; i < 8; ++i) { // Generate a random 8-character hex string
        ss << hex << rd();
    }

    string sessionId = ss.str();

    // Create and store the new session
    sessions[sessionId] = make_shared<GameSession>();

    return sessionId;
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
        // Assuming GameSession has a method to properly close or end the session
        // it->second->end(); // Uncomment if GameSession has such a method
        sessions.erase(it);
    }
}