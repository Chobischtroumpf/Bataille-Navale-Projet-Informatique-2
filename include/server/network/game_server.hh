#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <cpprest/http_listener.h>
#include <nlohmann/json.hpp>
#include <string>
#include "SessionManager.hpp" // Include SessionManager header
#include "TokenHandler.hpp"
#include "database.hh"
#include "queries.hh"
#include <memory>

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
    Queries dbManager; // Database interface instance
};

#endif // GAMESERVER_H
