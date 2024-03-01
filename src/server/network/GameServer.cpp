#include "GameServer.hpp"
#include <cpprest/http_listener.h>
#include <nlohmann/json.hpp>
#include <string>


using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using njson = nlohmann::json;


// Conversion method to utility::string_t type
// Shortcut as a lambda function
auto to_utf8 = [](const utility::string_t& input) -> std::string {
    return utility::conversions::to_utf8string(input);
};

auto to_string_t = [](const std::string& input) -> utility::string_t {
    return utility::conversions::to_string_t(input);
};

GameServer::GameServer(const std::string& address) : listener_(to_string_t(address)) {
    listener_.support(methods::GET, std::bind(&GameServer::handleGet, this, std::placeholders::_1));
    listener_.support(methods::POST, std::bind(&GameServer::handlePost, this, std::placeholders::_1));
}

GameServer::~GameServer() {
    // Destructor logic (if needed)
}

void GameServer::initialize() {
    try {
        listener_.open().wait();
        ucout << "GameServer is running at " << listener_.uri().to_string() << std::endl;

        // Initialize the session manager
        auto& sessionManager = SessionManager::getInstance();
        ucout << "Session Manager initialized" << std::endl;
    } catch (const std::exception& e) {
        ucerr << "Failed to open listener: " << e.what() << std::endl;
    }
}

bool GameServer::verifyAuthToken(const web::http::http_request& request) {
    auto headers = request.headers();
    if (headers.has(U("AuthToken"))) {
        auto authToken = headers[U("AuthToken")];
        
        // Placeholder logic for verifying the authToken
        
        bool isValid = true; // Placeholder for actual verification logic
        
        return isValid;

    } else {
        // If the AuthToken header is missing, return false
        return false;
    }
}

void GameServer::handleGet(http_request request) {
    auto path = request.relative_uri().path();
    njson response;

    // Retrieve the session manager instance
    auto& sessionManager = SessionManager::getInstance();


    // Handle the case for "/api/games" - List of session IDs
    if (path == U("/api/games")) {
        // Placeholder session IDs for demonstration
        std::vector<std::string> sessionIds = {"session1", "session2", "session3"};
        response["sessions"] = njson(sessionIds);
        request.reply(status_codes::OK, response.dump(), "application/json");
    }


    // Handle the case for "/api/games/{id}/query" - Query game state by session ID
    else if (path.find(U("/api/games/")) == 0 && path.find(U("/query")) != std::string::npos) {

        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
        auto userIdIt = queryParams.find(U("userid"));
        auto sessionId = "placeholder"; // Placeholder session ID shoudl be retrieved from the url

        //Verifying userId Iterator
        if (userIdIt != queryParams.end()) {
            auto userId = userIdIt->second;
            // Use userId to query the game state

            auto& gameSession = sessionManager.getSession(sessionId);
            njson gameState = gameSession->getGameState(to_utf8(userId));
            response["gameState"] = gameState;
            request.reply(status_codes::OK, response.dump(), "application/json");

        } else {
            // Handle missing userId parameter
            response["error"] = "Missing userId parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    }

    // Handle unmatched paths
    else {
        response["error"] = "Unknown path";
        request.reply(status_codes::BadRequest, response.dump(), "application/json");
    }
}

// Not operational, returns a basic response
void GameServer::handlePost(http_request request) {
    // Example: Handle game creation or player actions
    request.extract_json().then( [=](web::json::value requestBody) {
        njson response;
        // Process request and populate response
        response["message"] = "POST request received";
        request.reply(status_codes::OK, response.dump(), "application/json");
    }).wait();
}
