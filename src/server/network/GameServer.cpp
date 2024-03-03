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

GameServer::GameServer(const std::string& address) : listener_(to_string_t(address)), tokenHandler(), dbManager(database) {
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

// Verifies the validity of an auth token, and returns the userid if successful, otherwise returns an empty string
string GameServer::verifyAuthToken(const web::http::http_request& request) {
    auto headers = request.headers();

    if (headers.has(U("AuthToken"))) {
        auto authToken = headers[U("AuthToken")];
        
        // First, validate the token
        if (tokenHandler.validateToken(to_utf8(authToken))) {
            // If the token is valid, retrieve and return the user ID
            return tokenHandler.getUserID(to_utf8(authToken));
        } else {
            // If the token is invalid, return an empty string
            return "";
        }

    } else {
        // If the AuthToken header is missing, return an empty string
        return "";
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

        // Protected route - verify the AuthToken and retrieve the userId
        auto userId = verifyAuthToken(request);

        // If userId is empty, the token is invalid or missing
        if (userId.empty()) {
            response["error"] = "Invalid or missing AuthToken";
            request.reply(status_codes::Unauthorized, response.dump(), "application/json");
            return; // Stop further processing
        }

        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
         // Extracting sessionId  from query parameters
        auto sessionIdIt = queryParams.find(U("sessionid"));
        

        // Verifying both sessionId  are provided
        if (sessionIdIt != queryParams.end() ) {
            auto sessionId = sessionIdIt->second;
            

            // Use sessionId to query the game state
            auto gameSession = sessionManager.getSession(to_utf8(sessionId));
            njson gameState = gameSession->getGameState(userId);
            response["gameState"] = gameState;
            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing sessionId parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    }

    // Handle the case for "/api/games/join" - Join game by session ID ( and user ID ) -- Protected
    else if (path.find(U("/api/games/join")) == 0 ) {


         // Protected route - verify the AuthToken and retrieve the userId
        auto userId = verifyAuthToken(request);

        // If userId is empty, the token is invalid or missing
        if (userId.empty()) {
            response["error"] = "Invalid or missing AuthToken";
            request.reply(status_codes::Unauthorized, response.dump(), "application/json");
            return; // Stop further processing
        }

        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
         // Extracting sessionId from query parameters
        auto sessionIdIt = queryParams.find(U("sessionid"));
       

        // Verifying both sessionId and userId are provided
        if (sessionIdIt != queryParams.end() ) {
            auto sessionId = sessionIdIt->second;
           

            // Use sessionId and userId to add player to the game 
            auto gameSession = sessionManager.getSession(to_utf8(sessionId));
            gameSession->addParticipant(userId);

            response["gameDetails"] = {}; // Not implemented
            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing sessionId parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    }

    // Handle the case for "/api/user/uid" - Retrieve user ID from database
    else if (path.find(U("/api/login/uid")) == 0 ) {


        // Parsing query parameters
        auto queryParams = uri::split_query(request.request_uri().query());
         // Extracting sessionId from query parameters
        auto usernameIt = queryParams.find(U("username"));
       

        // Verifying both sessionId and userId are provided
        if (usernameIt != queryParams.end() ) {
            auto username = usernameIt->second;
           

            dbManager.checkUserName(to_utf8(username));

            
            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing username parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    }

    // Handle the case for "/api/chat/get" - Retrieve conversation with a user -- Protected
    if (path == U("/api/chat/get")) {
        
        // First, verify the AuthToken and retrieve the userId of the requester
        auto requesterId = verifyAuthToken(request);

        // If requesterId is empty, the token is invalid or missing
        if (requesterId.empty()) {
            response["error"] = "Invalid or missing AuthToken";
            request.reply(status_codes::Unauthorized, response.dump(), "application/json");
            return; // Stop further processing
        }

        // Parsing query parameters for the recipient's userId
        auto queryParams = uri::split_query(request.request_uri().query());
        auto recipientIdIt = queryParams.find(U("recipientID"));

        // Verifying recipient's userId is provided
        if (recipientIdIt != queryParams.end()) {
            auto recipientId = recipientIdIt->second;

            
            njson conversation; // Placeholder 
            
            bool retrievalResult = true;

            if (retrievalResult) {
                response["conversation"] = conversation;
                request.reply(status_codes::OK, response.dump(), "application/json");
            } else {
                response["error"] = "Failed to retrieve conversation";
                request.reply(status_codes::BadRequest, response.dump(), "application/json");
            }
        } else {
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

void GameServer::handlePost(http_request request) {
    auto path = request.relative_uri().path();
    
    // Retrieve the session manager instance
    auto& sessionManager = SessionManager::getInstance();

    request.extract_json().then([path, &sessionManager, this, request](web::json::value requestBody) mutable {
        njson response;

        // First, verify the AuthToken and retrieve the userId
        auto userId = verifyAuthToken(request);

        // If userId is empty, the token is invalid or missing
        if (userId.empty()) {
            response["error"] = "Invalid or missing AuthToken";
            request.reply(status_codes::Unauthorized, response.dump(), "application/json");
            return; // Stop further processing
        }

        // Handle the case for "/api/games/create" - Create a new game session -- Protected
        if (path == U("/api/games/create")) {
            // Extract game details from request body
            njson gameDetails = requestBody[U("gameDetails")].serialize();
            
            // Create a new session
            auto sessionId = njson::parse(sessionManager.createSession(userId, gameDetails));
            response["sessionId"] = sessionId;
            request.reply(status_codes::OK, response.dump(), "application/json");
        }
        
        // Handle the case for "/api/games/move" - Make a move in a game session -- Protected
        else if (path == U("/api/games/move")) {
            
            // First, verify the AuthToken and retrieve the userId
            auto userId = verifyAuthToken(request);

            // If userId is empty, the token is invalid or missing
            if (userId.empty()) {
                response["error"] = "Invalid or missing AuthToken";
                request.reply(status_codes::Unauthorized, response.dump(), "application/json");
                return; // Stop further processing
            }
            
            // Extract sessionId and move details from request body
            auto sessionId = requestBody[U("sessionId")].as_string();
            auto move = requestBody[U("move")].as_string();

            // Retrieve the session and make the move
            auto gameSession = sessionManager.getSession(to_utf8(sessionId));
            
            if (gameSession) {
                bool moveResult = gameSession->makeMove(userId, to_utf8(move));

                // Respond based on the result of the move
                if (moveResult) {
                    response["status"] = "Move successful";
                    request.reply(status_codes::OK, response.dump(), "application/json");
                } else {
                    response["error"] = "Move failed";
                    request.reply(status_codes::BadRequest, response.dump(), "application/json");
                }
            } else {
                response["error"] = "Session not found";
                request.reply(status_codes::NotFound, response.dump(), "application/json");
            }
        }

        // Handle the case for "/api/chat/send" - Send a message to a user -- Protected
        if (path == U("/api/chat/send")) {

            // First, verify the AuthToken and retrieve the userId
            auto userId = verifyAuthToken(request);

            // If userId is empty, the token is invalid or missing
            if (userId.empty()) {
                response["error"] = "Invalid or missing AuthToken";
                request.reply(status_codes::Unauthorized, response.dump(), "application/json");
                return; // Stop further processing
            }


            // Extract recipientId and message from request body
            auto recipientId = requestBody[U("recipientId")].as_string();
            auto message = requestBody[U("message")].as_string();

           
            bool sendMessageResult = true; // Placeholder 

            // Respond based on the result of sending the message
            if (sendMessageResult) {
                response["status"] = "Message sent successfully";
                request.reply(status_codes::OK, response.dump(), "application/json");
            } else {
                response["error"] = "Failed to send message";
                request.reply(status_codes::BadRequest, response.dump(), "application/json");
            }
        }

        // Handle the case for "/api/login" - User login 
        if (path == U("/api/login")) {

            // Extract userId from query parameters
            auto queryParams = uri::split_query(request.request_uri().query());
            auto usernameIt = queryParams.find(U("username"));
            if (usernameIt == queryParams.end()) {
                response["error"] = "Missing userId parameter";
                request.reply(status_codes::BadRequest, response.dump(), "application/json");
                return;
            }

            auto username = usernameIt->second;

            // Extract password from request body
            if (!requestBody.has_field(U("password"))) {
                response["error"] = "Missing password";
                request.reply(status_codes::BadRequest, response.dump(), "application/json");
                return;
            }

            auto password = requestBody[U("password")].as_string();


            // Authenticate user and generate authToken -------------------------------- TBA
            bool isSuccessful = this->dbManager.userLogin(to_utf8(username), to_utf8(password));

            if (!isSuccessful) {
                // Authentication failed
                response["error"] = "Authentication failed";
                request.reply(status_codes::Unauthorized, response.dump(), "application/json");
                return;       
            }

            string userId = this->dbManager.checkUserName(to_utf8(username)).getFirst();

            auto authToken = this->tokenHandler.generateToken(userId);

            if (!authToken.empty()) {
                // Authentication successful
                response["authToken"] = authToken;
                request.reply(status_codes::OK, response.dump(), "application/json");
            } else {
                // Authentication failed
                response["error"] = "Authentication failed";
                request.reply(status_codes::Unauthorized, response.dump(), "application/json");
            }
        }
         
        else {
            // Handle unmatched paths
            response["error"] = "Unknown path";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }

    });
}
