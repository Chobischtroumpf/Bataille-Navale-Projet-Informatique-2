#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <cpprest/http_listener.h>
#include <nlohmann/json.hpp>
#include <string>
#include "SessionManager.hpp" // Include SessionManager header

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using njson = nlohmann::json;

class GameServer {
public:
    GameServer() = delete; // Delete the default constructor
    explicit GameServer(const std::string& address);
    ~GameServer();
    
    bool verifyAuthToken(const web::http::http_request& request);

    void initialize();
    void handleGet(http_request request);
    void handlePost(http_request request);

private:
    http_listener listener_; // HTTP listener
};

#endif // GAMESERVER_H
