#include "GameServer.hpp"

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

GameServer::GameServer(const std::string& address) : listener_(to_string_t(address)), tokenHandler() {
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
        
        return tokenHandler.validateToken(authToken);

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


    // Handle the case for "/api/games/query" - Query game state by session ID ( and user ID ) -- Protected
    else if (path.find(U("/api/games/query")) == 0 ) {



        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
         // Extracting sessionId and userId from query parameters
        auto sessionIdIt = queryParams.find(U("sessionid"));
        auto userIdIt = queryParams.find(U("userid"));

        // Verifying both sessionId and userId are provided
        if (sessionIdIt != queryParams.end() && userIdIt != queryParams.end()) {
            auto sessionId = sessionIdIt->second;
            auto userId = userIdIt->second;

            // Use sessionId and userId to query the game state
            auto gameSession = sessionManager.getSession(to_utf8(sessionId));
            njson gameState = gameSession->getGameState(to_utf8(userId));
            response["gameState"] = gameState;
            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing sessionId or userId parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    }

    // Handle the case for "/api/games/join" - Join game by session ID ( and user ID ) -- Protected
    else if (path.find(U("/api/games/join")) == 0 ) {

        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
         // Extracting sessionId and userId from query parameters
        auto sessionIdIt = queryParams.find(U("sessionid"));
        auto userIdIt = queryParams.find(U("userid"));

        // Verifying both sessionId and userId are provided
        if (sessionIdIt != queryParams.end() && userIdIt != queryParams.end()) {
            auto sessionId = sessionIdIt->second;
            auto userId = userIdIt->second;

            // Use sessionId and userId to add player to the game 
            auto gameSession = sessionManager.getSession(to_utf8(sessionId));
            gameSession->addParticipant(to_utf8(userId));

            response["gameDetails"] = {}; // Not implemented
            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing sessionId or userId parameter";
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
