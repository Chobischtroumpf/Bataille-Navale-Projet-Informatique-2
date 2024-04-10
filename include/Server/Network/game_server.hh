#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <unordered_map>
#include <vector>

#include "session_manager.hh" // Include SessionManager header
#include "token_handler.hh"
#include "database.hh"
#include "queries.hh"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using njson = nlohmann::json;

class GameServer {
public:
    GameServer() = delete; // Delete the default constructor
    explicit GameServer(const std::string& address);
    ~GameServer();
    
    std::string verifyAuthToken(const web::http::http_request& request);

    void initialize();
    void handleGet(http_request request);
    void handlePost(http_request request);

private:
    http_listener listener_; // HTTP listener
    TokenHandler tokenHandler; // TokenHandler instance
    std::shared_ptr<DataBase> database; // DataBase instance
    std::unordered_map<std::string, std::vector<std::string>> users_notifications;
    Queries dbManager; // Database interface instance

    // Post requests handlers
    void handleCreateGameRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleMoveRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleAddFriendRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleLoginRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleRegisterRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleMessageSendRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleAddNotificationRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager);

    // Get requests handlers
    void handleGetGames(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleQueryGame(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleJoinGame(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleGetUID(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleGetUsername(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleGetChat(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleGetFriendList(const string& path, http_request& request, njson response, SessionManager& sessionManager);
    void handleGetNotification(const string& path, http_request& request, njson response, SessionManager& sessionManager);


};
