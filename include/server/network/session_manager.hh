#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <unordered_map>
#include <mutex>
#include <memory>
#include "game_session.hh" // Ensure you have a GameSession class defined
#include <nlohmann/json.hpp>
using njson = nlohmann::json;

using namespace std;

class SessionManager {
public:
    // Deletes copy constructor and assignment operator to ensure singleton uniqueness
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    // Static method to get the singleton instance
    static SessionManager& getInstance();

    // Method to create a new game session, returns the session ID
    string createSession( const string& userId, const njson& gameDetails);

    // Method to retrieve a session by its ID
    shared_ptr<GameSession> getSession(const string& sessionId);

    // Method to end a session and remove it from the manager
    void endSession(const string& sessionId);

private:
    // Private constructor for singleton pattern
    SessionManager();

    // Mutex for thread-safe access to the sessions container
    mutex sessionsMutex;

    // Container for active sessions
    unordered_map<string, shared_ptr<GameSession>> sessions;
};

#endif // SESSIONMANAGER_H