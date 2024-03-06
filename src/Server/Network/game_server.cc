#include "game_server.hh"

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

GameServer::GameServer(const std::string& address) : listener_(to_string_t(address)), tokenHandler(), database(std::make_shared<DataBase>()), dbManager(database) {
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
    cout << "Verifying AuthToken..." << endl;
    auto headers = request.headers();

    if (headers.has(U("Authorization"))) {
        cout << "Authorization header found." << endl;
        auto authHeader = headers[U("Authorization")];
        
        // Expected format: "Bearer <token>"
        auto authHeaderStr = utility::conversions::to_utf8string(authHeader);
        if (authHeaderStr.rfind("Bearer ", 0) == 0) { // Check if the string starts with "Bearer "
            auto authToken = authHeaderStr.substr(7); // Extract token after "Bearer "
            cout << "Extracted AuthToken: " << authToken << endl;

            // Validate the token
            if (tokenHandler.validateToken(authToken)) {
                cout << "AuthToken is valid." << endl;
                // If the token is valid, retrieve and return the user ID
                auto userId = tokenHandler.getUserID(authToken);
                cout << "UserID retrieved: " << userId << endl;
                return userId;
            } else {
                cout << "AuthToken is invalid." << endl;
                // If the token is invalid, return an empty string
                return "";
            }
        } else {
            cout << "Authorization header does not contain a bearer token." << endl;
            return "";
        }

    } else {
        cout << "Authorization header is missing." << endl;
        // If the Authorization header is missing, return an empty string
        return "";
    }
}

void GameServer::handleGet(http_request request) {
    try {
        auto path = request.relative_uri().path();
        njson response;

        cout << "Received GET request for path " << to_utf8(path) << endl;

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
        else if (path.find(U("/api/games/query")) != wstring::npos ) {

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
                
                if( !sessionManager.sessionExists(to_utf8(sessionId)) ) {
                    // Handle missing parameters
                    response["error"] = "Session could not be found";
                    request.reply(status_codes::BadRequest, response.dump(), "application/json");
                }

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
        else if (path.find(U("/api/games/join")) != wstring::npos) {


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
        else if (path.find(U("/api/login/uid")) != wstring::npos ) {


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
        else if (path.find(U("/api/chat/get")) != wstring::npos ) {
            
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

                
                QueryResult result = this->dbManager.getMsgBetweenUsers(requesterId, to_utf8(recipientId));

                if (!result.isOk()) {
                    response["error"] = "Failed to retrieve conversation : " + result.getError();
                    request.reply(status_codes::BadRequest, response.dump(), "application/json");
                }

                njson conversation = njson::array();
                
                njson messagesJson ; // Create a JSON array to hold the messages

                // Iterate over each message in the QueryResult
                for (const auto& messageVec : result.data) {
                    if (messageVec.size() == 3) { // Ensure the inner vector has exactly 3 elements
                        // Create a JSON object for the current message
                        njson messageJson = {
                            {"sender", messageVec[0]},
                            {"message", messageVec[2]}
                        };
                        // Append the message JSON object to the messages array
                        messagesJson.push_back(messageJson);
                    }
                }
                
                response["conversation"] = conversation;
                request.reply(status_codes::OK, response.dump(), "application/json");
                
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
    } catch (const exception& e) {
                // In case of exception, set an error value
                cerr << "Exception in handleGet: " << e.what() << endl;
    }

}

void GameServer::handlePost(http_request request) {
    try {
            auto path = request.relative_uri().path();
            
            cout << "Received POST request for path " << to_utf8(path) << endl;

            // Retrieve the session manager instance
            auto& sessionManager = SessionManager::getInstance();

            request.extract_json().then([path, &sessionManager, this, request](web::json::value requestBody) mutable {
                njson response;

                // Handle the case for "/api/games/create" - Create a new game session -- Protected
                if (path.find(U("/api/games/create")) != wstring::npos) {
                    
                    // First, verify the AuthToken and retrieve the userId
                    auto userId = verifyAuthToken(request);
                    // If userId is empty, the token is invalid or missing
                    if (userId.empty()) {
                        response["error"] = "Invalid or missing AuthToken";
                        request.reply(status_codes::Unauthorized, response.dump(), "application/json");
                        return; // Stop further processing
                    }

                     // Check if "gameDetails" exists in the requestBody
                    if (!requestBody.has_field(U("gameDetails"))) {
                        response["error"] = "Missing gameDetails";
                        request.reply(status_codes::BadRequest, response.dump(), "application/json");
                        return; 
                    }

                    // Extract game details from request body
                    njson gameDetails = njson::parse(requestBody[U("gameDetails")].serialize());

                    // Create a new session
                    auto sessionId = njson::parse(sessionManager.createSession(userId, gameDetails));
                    response["sessionId"] = sessionId;
                    request.reply(status_codes::OK, response.dump(), "application/json");
                }
                
                // Handle the case for "/api/games/move" - Make a move in a game session -- Protected
                else if (path.find(U("/api/games/move")) != wstring::npos ) {
                    
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
                else if (path.find(U("/api/chat/send")) != wstring::npos ) {

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
                else if (path.find(U("/api/login")) != wstring::npos ) {
                    
                    // Extract username and password from request body
                    if (!requestBody.has_field(U("username")) || !requestBody.has_field(U("password"))) {
                        response["error"] = "Missing username or password";
                        request.reply(status_codes::BadRequest, response.dump(), "application/json");
                        return;
                    }
                    auto username = requestBody[U("username")].as_string();
                    auto password = requestBody[U("password")].as_string();

                    cout << "Username: " + to_utf8(username) + "  Password: " + to_utf8(password) << endl;

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
                        // AuthToken generation failed
                        response["error"] = "AuthToken generation failed";
                        request.reply(status_codes::InternalError, response.dump(), "application/json");
                    }
                }

                // Handle the case for "/api/register" - User registration
                else if (path.find(U("/api/register")) != wstring::npos ) {

                    // Extract username and password from request body
                    if (!requestBody.has_field(U("username")) || !requestBody.has_field(U("password"))) {
                        response["error"] = "Missing username or password";
                        request.reply(status_codes::BadRequest, response.dump(), "application/json");
                        return;
                    }

                    auto username = requestBody[U("username")].as_string();
                    auto password = requestBody[U("password")].as_string();

                    cout << "Username: " + to_utf8(username) + "  Password: " + to_utf8(password) << endl;

                    // Register user ------------------------------------------------ TBA
                    bool isRegistered = this->dbManager.userRegister(to_utf8(username), to_utf8(password));

                    if (!isRegistered) {
                        // Registration failed
                        response["error"] = "Registration failed";
                        request.reply(status_codes::BadRequest, response.dump(), "application/json");
                        return;
                    }

                    // After successful registration, generate authToken for the user
                    string userId = this->dbManager.checkUserName(to_utf8(username)).getFirst();

                    auto authToken = this->tokenHandler.generateToken(userId);

                    if (!authToken.empty()) {
                        // Registration and authToken generation successful
                        response["authToken"] = authToken;
                        request.reply(status_codes::OK, response.dump(), "application/json");
                    } else {
                        // AuthToken generation failed
                        response["error"] = "AuthToken generation failed";
                        request.reply(status_codes::InternalError, response.dump(), "application/json");
                    }
                }

                // Handle the case for "/api/chat/send" - Sending a message
                else if (path.find(U("/api/chat/send")) != wstring::npos ) {
                    // Protected route - verify the AuthToken and retrieve the senderId
                    auto senderId = verifyAuthToken(request);

                    // If senderId is empty, the token is invalid or missing
                    if (senderId.empty()) {
                        response["error"] = "Invalid or missing AuthToken";
                        request.reply(status_codes::Unauthorized, response.dump(), "application/json");
                        return; // Stop further processing
                    }

                    // Extract recipientId and message from request body
                    auto recipientId = requestBody[U("recipientId")].as_string();
                    auto message = requestBody[U("message")].as_string();

                    // Send the message using your backend logic -------------------------------- TBA
                    bool isSuccessful = this->dbManager.sendMsg(senderId, to_utf8(recipientId), to_utf8(message));

                    if (isSuccessful) {
                        // Message sent successfully
                        response["status"] = "Message sent successfully";
                        request.reply(status_codes::OK, response.dump(), "application/json");
                    } else {
                        // Failed to send message
                        response["error"] = "Failed to send message";
                        request.reply(status_codes::BadRequest, response.dump(), "application/json");
                    }
                }

                else {
                    // Handle unmatched paths
                    response["error"] = "Unknown path";
                    request.reply(status_codes::BadRequest, response.dump(), "application/json");
                }
             }).then([](pplx::task<void> errorHandler) {
                try {
                    // Attempt to catch exceptions if any
                    errorHandler.get();
                } catch (const exception& e) {
                    // In case of exception, set an error value
                    cerr << "Exception in handlePost (lambda): " << e.what() << endl;
                }
            });

    } catch (const std::exception& e) {
        cerr << "Exception in handlePost : " << e.what() << endl;
    }
}
