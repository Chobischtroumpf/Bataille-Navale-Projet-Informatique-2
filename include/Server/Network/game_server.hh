#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <unordered_map>
#include <vector>

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
};
