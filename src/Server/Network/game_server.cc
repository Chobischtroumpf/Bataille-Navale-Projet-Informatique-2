
#include "session_manager.hh" // Include SessionManager header
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
        std::clog << "GameServer is running at " << listener_.uri().to_string() << std::endl;
        std::cout << "GameServer is running at " << listener_.uri().to_string() << std::endl;
        // Initialize the session manager
        auto& sessionManager = SessionManager::getInstance();
        std::clog << "Session Manager initialized" << std::endl;
        ucout << "Session Manager initialized" << std::endl;

    } catch (const std::exception& e) {
        ucerr << "Failed to open listener: " << e.what() << std::endl;
        std::clog << "Failed to open listener: " << e.what() << std::endl;
        throw;
    }
}

// Verifies the validity of an auth token, and returns the userid if successful, otherwise returns an empty string
string GameServer::verifyAuthToken(const web::http::http_request& request) {
    std::clog << "Verifying AuthToken..." <<std::endl;
    auto headers = request.headers();

    if (headers.has(U("Authorization"))) {
        std::clog << "Authorization header found." <<std::endl;
        auto authHeader = headers[U("Authorization")];
        
        // Expected format: "Bearer <token>"
        auto authHeaderStr = utility::conversions::to_utf8string(authHeader);
        if (authHeaderStr.rfind("Bearer ", 0) == 0) { // Check if the string starts with "Bearer "
            auto authToken = authHeaderStr.substr(7); // Extract token after "Bearer "
            std::clog << "Extracted AuthToken: " << authToken <<std::endl;

            // Validate the token
            if (tokenHandler.validateToken(authToken)) {
                std::clog << "AuthToken is valid." <<std::endl;
                // If the token is valid, retrieve and return the user ID
                auto userId = tokenHandler.getUserID(authToken);
                std::clog << "UserID retrieved: " << userId <<std::endl;
                return userId;
            } else {
                std::clog << "AuthToken is invalid." <<std::endl;
                // If the token is invalid, return an empty string
                return "";
            }
        } else {
            std::clog << "Authorization header does not contain a bearer token." <<std::endl;
            return "";
        }

    } else {
        std::clog << "Authorization header is missing." <<std::endl;
        // If the Authorization header is missing, return an empty string
        return "";
    }
}

void GameServer::handleGet(http_request request) {
      try {
        auto path = request.relative_uri().path();
        njson response;

        std::cout << "Received GET request for path " << to_utf8(path) <<std::endl;
        std::clog << "Received GET request for path " << to_utf8(path) <<std::endl;

        // Retrieve the session manager instance
        auto& sessionManager = SessionManager::getInstance();

        // Handle the case for "/api/games" - List of session IDs
        if (path == U("/api/games")) {
           handleGetGames(path, request, response, sessionManager);
        }

        // Handle the case for "/api/games/query" - Query game state by session ID ( and user ID ) -- Protected
        else if (path.find(U("/api/games/query")) != std::wstring::npos ) {
            handleQueryGame(path, request, response, sessionManager);
        }

        // Handle the case for "/api/games/join" - Join game by session ID ( and user ID ) -- Protected
        else if (path.find(U("/api/games/join")) != std::wstring::npos) {
          handleJoinGame(path, request, response, sessionManager);
        }

        // Handle the case for "/api/games/join" - Join game by session ID ( and user ID ) -- Protected
        else if (path.find(U("/api/games/history")) != std::wstring::npos) {
          handleGetHistory(path, request, response, sessionManager);
        }
        // Handle the case for "/api/user/uid" - Retrieve user ID from database
        else if (path.find(U("/api/login/uid")) != std::wstring::npos ) {
          handleGetUID(path, request, response, sessionManager);
        }

        // Handle the case for "/api/username" - Retrieve username from database
        else if (path.find(U("/api/username")) != std::wstring::npos ) {
          handleGetUsername(path, request, response, sessionManager);
        }

        // Handle the case for "/api/chat/get" - Retrieve conversation with a user -- Protected
        else if (path.find(U("/api/chat/get")) != std::wstring::npos ) {
          handleGetChat(path, request, response, sessionManager);
        }
        
        // Handle the case for "/api/friend/list" - Retrieving the friend list -- Protected
        else if (path.find(U("/api/friend/list")) != std::wstring::npos) {
          handleGetFriendList(path, request, response, sessionManager);
        }

        // Handle the case for "/api/notification/get" - Retrieving the user's notifications -- Protected
        else if (path == U("/api/notification/get")) {
          handleGetNotification(path, request, response, sessionManager);
        }

        // Handle unmatched paths
        else {
            response["error"] = "Unknown path";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    } catch (const std::exception& e) {
                // In case of std::exception, set an error value
                std::clog << "Exception in handleGet: " << e.what() <<std::endl;
    }
}

void GameServer::handleGetGames(const string& path, http_request& request, njson response, SessionManager& sessionManager){

    // Protected route - verify the AuthToken and retrieve the userId
    auto userId = verifyAuthToken(request);

    // If userId is empty, the token is invalid or missing
    if (userId.empty()) {
        response["error"] = "Invalid or missing AuthToken";
        request.reply(status_codes::Unauthorized, response.dump(), "application/json");
        return; // Stop further processing
    }

    // Placeholder session IDs for demonstration
    auto query = dbManager.getSessionId(userId);

    if ( !query.isOk()) {
      std::cout << query.getError() << std::endl;
      return;
    }

    if ( query.data.size() < 1 ) {
      std::cout << "No games found" << std::endl;
      response["sessions"] = njson{};
      request.reply(status_codes::OK, response.dump(), "application/json");
      return;
    }

    std::vector<std::vector<std::string>> sessionsData = query.data;

    // Initialize the sessions array in the JSON response
    response["sessions"] = nlohmann::json::array();

    // Iterate over all sessions
    for (const auto& session : sessionsData) {
        if (session.size() >= 3) {  // Check if the session vector has at least 3 elements
            nlohmann::json sessionDetails;
            sessionDetails["sessionId"] = session[0];
            sessionDetails["player1Id"] = session[1];
            sessionDetails["player2Id"] = session[2];

            response["sessions"].push_back(sessionDetails);
        } else {
            std::cerr << "Error: Session data is incomplete." << std::endl;
        }
    }
   
    request.reply(status_codes::OK, response.dump(), "application/json");
}

void GameServer::handleQueryGame(const string& path, http_request& request, njson response, SessionManager& sessionManager){
  try {
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
    auto sessionIdIt = queryParams.find(U("session_id"));
    
    // Verifying both sessionId  are provided
    if (sessionIdIt != queryParams.end() ) {
        auto sessionId = sessionIdIt->second;
        
        if( !sessionManager.sessionExists(to_utf8(sessionId)) ) {
            // Handle missing parameters
            response["error"] = "Session could not be found";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
            return;
        }

        // Use sessionId to query the game state
        auto gameSession = sessionManager.getSession(to_utf8(sessionId));
        njson gameState = gameSession->getGameState(userId);
        njson gameDetails;

        gameDetails["gameState"] = gameState;
        auto sessionState = gameSession->getSessionState();

        gameDetails["participants"] = sessionState["participants"];
        gameDetails["hasStarted"] = sessionState["hasStarted"];
        gameDetails["sessionName"] = sessionState["sessionName"];
        response["gameDetails"] = gameDetails;

        request.reply(status_codes::OK, response.dump(), "application/json");
    } else {
        // Handle missing parameters
        response["error"] = "Missing sessionId parameter";
        request.reply(status_codes::BadRequest, response.dump(), "application/json");
    }
  } catch (const std::exception &e) {
    // In case of exception, set an error value
    std::cerr << "Exception in api/games/query/: " << e.what() << std::endl;
  }
}

void GameServer::handleGetHistory(const string& path, http_request& request, njson response, SessionManager& sessionManager) {
    try {
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
        auto sessionIdIt = queryParams.find(U("session_id"));

        // Verifying sessionId is provided
        if (sessionIdIt != queryParams.end()) {
            auto sessionId = sessionIdIt->second;

            // Check if session exists ?

            // Retrieve game history from the database using dbManager
            auto query = dbManager.getGameStates(to_utf8(sessionId));

            if ( !query.isOk()) {
              std::cout << query.getError() << std::endl;
              return;
            }
            
            std::cout << "query successful : " << query.isOk() << "size: " << query.data.size() << std::endl;
            auto states = query.data[0];

            njson gameHistory;
            gameHistory["states"] = njson::array();

            // Loop through each state string in the vector
            for (const auto& statestr : states) {
                // Parse each state string into an njson object and append to the 'states' array
                njson stateJson = njson::parse(statestr);
                gameHistory["states"].push_back(stateJson);
            }

            // Add the game history to the response
            response["gameHistory"] = gameHistory;

            request.reply(status_codes::OK, response.dump(), "application/json");
        } else {
            // Handle missing parameters
            response["error"] = "Missing sessionId parameter";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
        }
    } catch (const std::exception &e) {
        // In case of exception, set an error value
        std::clog << "Exception in api/games/history/: " << e.what() << std::endl;
    }
}

void GameServer::handleJoinGame(const string& path, http_request& request, njson response, SessionManager& sessionManager) {
  try {
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
    auto sessionIdIt = queryParams.find(U("session_id"));


    // Verifying both sessionId and userId are provided
    if (sessionIdIt != queryParams.end() ) {
        auto sessionId = sessionIdIt->second;
        
        //Check if session exists
        if( !sessionManager.sessionExists(to_utf8(sessionId)) ) {
            // Return error
            response["error"] = "Session could not be found";
            request.reply(status_codes::BadRequest, response.dump(), "application/json");
            return;
        }

        // Use sessionId and userId to add player to the game 
        auto gameSession = sessionManager.getSession(to_utf8(sessionId));
        auto result = gameSession->addParticipant(userId);

        response["isSuccessful"] = result; 
        request.reply(status_codes::OK, response.dump(), "application/json");
    } else {
        // Handle missing parameters
        response["error"] = "Missing sessionId parameter";
        request.reply(status_codes::BadRequest, response.dump(), "application/json");
    }

  } catch (const std::exception &e) {
    // In case of exception, set an error value
    std::cerr << "Exception in api/games/join/: " << e.what() << std::endl;
  }
}

void GameServer::handleGetUID(const string& path, http_request& request, njson response, SessionManager& sessionManager) {
  try {
    // Parsing query parameters
    auto queryParams = uri::split_query(request.request_uri().query());
    // Extracting sessionId from query parameters
    auto usernameIt = queryParams.find(U("username"));


    // Verifying both sessionId and userId are provided
    if (usernameIt != queryParams.end() ) {
        auto username = usernameIt->second;

        QueryResult query = dbManager.checkUserName(to_utf8(username));

        if (!query.isOk()) {
            // Handle missing parameters
            response["error"] = "Error on parameter query : " + query.getError();
            request.reply(status_codes::InternalError, response.dump(), "application/json");
            return;
        }

        string userId = query.getFirst();
        response["userId"] = userId;

        request.reply(status_codes::OK, response.dump(), "application/json");
    } else {
        // Handle missing parameters
        response["error"] = "Missing username parameter";
        request.reply(status_codes::BadRequest, response.dump(), "application/json");
    }
  } catch (const std::exception &e) {
    // In case of exception, set an error value
    std::cerr << "Exception in api/login/uid/: " << e.what() << std::endl;
  }
}

void GameServer::handleGetUsername(const string& path, http_request& request, njson response, SessionManager& sessionManager){
  try {
      // Parsing query parameters
      auto queryParams = uri::split_query(request.request_uri().query());
      // Extracting sessionId from query parameters
      auto userIdIt = queryParams.find(U("userId"));
  

      // Verifying both sessionId and userId are provided
      if (userIdIt != queryParams.end() ) {
          auto userId = userIdIt->second;

          QueryResult query = dbManager.getUsername(to_utf8(userId));

          if (!query.isOk()) {
              // Handle missing parameters
              response["error"] = "Error on parameter query : " + query.getError();
              request.reply(status_codes::InternalError, response.dump(), "application/json");
          }

          string username = query.getFirst();
          response["username"] = username;

          request.reply(status_codes::OK, response.dump(), "application/json");
      } else {
          // Handle missing parameters
          response["error"] = "Missing username parameter";
          request.reply(status_codes::BadRequest, response.dump(), "application/json");
      }
    } catch (const std::exception &e) {
      // In case of exception, set an error value
      std::cerr << "Exception in api/username/: " << e.what() << std::endl;
    }
}

void GameServer::handleGetChat(const string& path, http_request& request, njson response, SessionManager& sessionManager){
  try { 
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
      auto recipientIdIt = queryParams.find(U("recipientId"));

      // Verifying recipient's userId is provided
      if (recipientIdIt != queryParams.end()) {
          auto recipientId = recipientIdIt->second;

          
          QueryResult result = this->dbManager.getMsgBetweenUsers(requesterId, to_utf8(recipientId));

          if (!result.isOk()) {
              response["error"] = "Failed to retrieve conversation : " + result.getError();
              request.reply(status_codes::BadRequest, response.dump(), "application/json");
          }

          njson conversation = njson::array(); // Create a JSON array to hold the messages
          
          // Iterate over each message in the QueryResult
          for (const auto& messageVec : result.data) {
              if (messageVec.size() == 3) { // Ensure the inner vector has exactly 3 elements
                  // Create a JSON object for the current message
                  njson messageJson = {
                      {"sender", messageVec[0]},
                      {"message", messageVec[2]}
                  };
                  // Append the message JSON object to the messages array
                  conversation.push_back(messageJson);
              }
          }
          
          response["conversation"] = conversation;
          request.reply(status_codes::OK, response.dump(), "application/json");
          
      } else {
          response["error"] = "Missing recipientId parameter";
          request.reply(status_codes::BadRequest, response.dump(), "application/json");
      }
  } catch (const std::exception &e) {
    // In case of exception, set an error value
    std::cerr << "Exception in api/chat/get/: " << e.what() << std::endl;
  }
}

void GameServer::handleGetFriendList(const string& path, http_request& request, njson response, SessionManager& sessionManager){
  try {
    // Protected route - verify the AuthToken and retrieve the userId
    auto userId = verifyAuthToken(request);

    // If userId is empty, the token is invalid or missing
    if (userId.empty()) {
        response["error"] = "Invalid or missing AuthToken";
        request.reply(status_codes::Unauthorized, response.dump(), "application/json");
        return; // Stop further processing
    }

    // Retrieve the user's friend list
    auto friendQuery = dbManager.getUserFriends(userId);

    if (!friendQuery.isOk()) {
        // Handle error in retrieving friend list
        response["error"] = "Failed to retrieve friend list";
        request.reply(status_codes::BadRequest, response.dump(), "application/json");
    } else {
        // Successfully retrieved friend list
        nlohmann::json friendsJson = nlohmann::json::array();
        for (const auto& friendId : friendQuery.data) {
            friendsJson.push_back(friendId);
        }
        response["friends"] = friendsJson;
        request.reply(status_codes::OK, response.dump(), "application/json");
    }
  } catch (const std::exception &e) {
    // In case of exception, set an error value
    std::cerr << "Exception in api/friend/list: " << e.what() << std::endl;
  }
}

void GameServer::handleGetNotification(const string& path, http_request& request, njson response, SessionManager& sessionManager){
  // First, verify the AuthToken and retrieve the userId
  auto userId = verifyAuthToken(request);

  if (userId.empty()) {
      response["error"] = "Invalid or missing AuthToken";
      request.reply(status_codes::Unauthorized, response.dump(), "application/json");
      return; // Stop further processing
  }

  QueryResult username = dbManager.getUsername(userId);
  std::string real_username = username.getFirst();

  std::vector<std::string> notificationsVector = users_notifications[real_username];

  // Successfully retrieved user's notifications
  nlohmann::json notifsJson = nlohmann::json::array();
  for (const auto& notification : notificationsVector) {
      notifsJson.push_back(notification);
  }
  response["notifications"] = notifsJson;
  request.reply(status_codes::OK, response.dump(), "application/json");
}


void GameServer::handlePost(http_request request) {
  try {
    auto path = request.relative_uri().path();

    std::cout << "Received POST request for path " << to_utf8(path) <<std::endl;
    std::clog << "Received POST request for path " << to_utf8(path) <<std::endl;

    // Retrieve the session manager instance
    auto &sessionManager = SessionManager::getInstance();
    

    request.extract_json()
        .then([path, &sessionManager, this,
               request](web::json::value requestBody) mutable {
          
          njson response;
          // Handle the case for "/api/games/create" - Create a new game session
          // -- Protected
          if (path.find(U("/api/games/create")) != std::wstring::npos) {

            handleCreateGameRequest(requestBody, path, request, response, sessionManager);
          }

          // Handle the case for "/api/games/move" - Make a move in a game
          // session -- Protected
          else if (path.find(U("/api/games/move")) != std::wstring::npos) {

           handleMoveRequest(requestBody, path, request, response, sessionManager);
          }

          // Handle the case for "/api/friend/add" - Add a friend
          else if (path == U("/api/friend/add")) {

            handleAddFriendRequest(requestBody, path, request, response, sessionManager);
          }

          // Handle the case for "/api/login" - User login
          else if (path.find(U("/api/login")) != std::wstring::npos) {

            handleLoginRequest(requestBody, path, request, response, sessionManager);
          }

          // Handle the case for "/api/register" - User registration
          else if (path.find(U("/api/register")) != std::wstring::npos) {

            handleRegisterRequest(requestBody, path, request, response, sessionManager);
          }

          // Handle the case for "/api/chat/send" - Sending a message
          else if (path.find(U("/api/chat/send")) != std::wstring::npos) {
            handleMessageSendRequest(requestBody, path, request, response, sessionManager);
          }

          else if (path.find(U("/api/user/add/notification")) != std::wstring::npos) {
            handleAddNotificationRequest(requestBody, path, request, response, sessionManager);
          }

          else {
            // Handle unmatched paths
            response["error"] = "Unknown path";
            request.reply(status_codes::BadRequest, response.dump(),
                          "application/json");
          }

        })
        .then([](pplx::task<void> errorHandler) {
          try {
            // Attempt to catch exceptions if any
            errorHandler.get();
          } catch (const std::exception &e) {
            // In case of std::exception, set an error value
            std::clog << "Exception in handlePost (lambda): " << e.what() <<std::endl;
          }
        });

  } catch (const std::exception &e) {
    std::clog << "Exception in handlePost : " << e.what() <<std::endl;
  }
}

void GameServer::handleCreateGameRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager) {
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
  auto sessionId = sessionManager.createSession(userId, gameDetails, this->dbManager);
  response["session_id"] = sessionId;
  request.reply(status_codes::OK, response.dump(),"application/json");
}

void GameServer::handleMoveRequest(web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager) { 
  // First, verify the AuthToken and retrieve the userId
  auto userId = verifyAuthToken(request);

  // If userId is empty, the token is invalid or missing
  if (userId.empty()) {
    response["error"] = "Invalid or missing AuthToken";
    request.reply(status_codes::Unauthorized, response.dump(),
                  "application/json");
    return; // Stop further processing
  }
  std::string sessionId;
  try {
    // Extract sessionId and move details from request body
    sessionId = requestBody[U("session_id")].as_string();
  } catch ( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }
  
  njson move = njson::parse(requestBody[U("move")].serialize());
  // Retrieve the session and make the move
  auto gameSession = sessionManager.getSession(to_utf8(sessionId));
  if (gameSession) {
    bool moveResult = gameSession->makeMove(userId, move);
    // Respond based on the result of the move
    if (moveResult) {
      response["status"] = "Move successful";
      request.reply(status_codes::OK, response.dump(),
                    "ap plication/json");
    } else {
      response["error"] = "Move failed";
      request.reply(status_codes::BadRequest, response.dump(),
                    "application/json");
    }
  } else {
    response["error"] = "Session not found";
    request.reply(status_codes::NotFound, response.dump(),
                  "application/json");
  }
}

void GameServer::handleAddFriendRequest(web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager) {
  // First, verify the AuthToken and retrieve the userId
  auto userId = verifyAuthToken(request);
  std::cout << "user : " << userId << std::endl;
  std::cout << "request : " << path << std::endl;

  // If userId is empty, the token is invalid or missing
  if (userId.empty()) {
    response["error"] = "Invalid or missing AuthToken";
    request.reply(status_codes::Unauthorized, response.dump(),
                  "application/json");
    return; // Stop further processing
  }

  // Extract friendUsername from request body
  auto friendUsername = requestBody[U("friendUsername")].as_string();
  // Response based on the friend's username validity
  if (dbManager.checkUserName(friendUsername).isOk()) {
    // Adding the friend
    dbManager.addFriend(userId, friendUsername);
    response["status"] = "Friend added successfully";
    request.reply(status_codes::OK, response.dump(),
                  "application/json");
  } else {
    response["error"] = "Failed to add friend";
    request.reply(status_codes::BadRequest, response.dump(),
                  "application/json");
  }
}

void GameServer::handleLoginRequest(web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager){
// Extract username and password from request body
  if (!requestBody.has_field(U("username")) ||
      !requestBody.has_field(U("password"))) {
    response["error"] = "Missing username or password";
    request.reply(status_codes::BadRequest, response.dump(),
                  "application/json");
    return;
  }
  auto username = requestBody[U("username")].as_string();
  auto password = requestBody[U("password")].as_string();

  std::cout << "Username: " + to_utf8(username) +
              "  Password: " + to_utf8(password)
        << std::endl;

  // Authenticate user and generate authToken
  // -------------------------------- TBA
  bool isSuccessful =
      this->dbManager.userLogin(to_utf8(username), to_utf8(password));

  if (!isSuccessful) {
    // Authentication failed
    response["error"] = "Authentication failed";
    request.reply(status_codes::Unauthorized, response.dump(),
                  "application/json");
    return;
  }

  string userId =
      this->dbManager.checkUserName(to_utf8(username)).getFirst();

  auto authToken = this->tokenHandler.generateToken(userId);

  if (!authToken.empty()) {
    // Authentication successful
    response["authToken"] = authToken;
    request.reply(status_codes::OK, response.dump(),
                  "application/json");
  } else {
    // AuthToken generation failed
    response["error"] = "AuthToken generation failed";
    request.reply(status_codes::InternalError, response.dump(),
                  "application/json");
  }
}

void GameServer::handleRegisterRequest( web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager){
  // Extract username and password from request body
  if (!requestBody.has_field(U("username")) ||
      !requestBody.has_field(U("password"))) {
    response["error"] = "Missing username or password";
    request.reply(status_codes::BadRequest, response.dump(),
                  "application/json");
    return;
  }

  auto username = requestBody[U("username")].as_string();
  auto password = requestBody[U("password")].as_string();

  std::cout << "Username: " + to_utf8(username) +
              "  Password: " + to_utf8(password)
        << std::endl;

  // Register user ------------------------------------------------
  // TBA
  bool isRegistered = this->dbManager.userRegister(to_utf8(username),
                                                    to_utf8(password));

  if (!isRegistered) {
    // Registration failed
    response["error"] = "Registration failed";
    request.reply(status_codes::BadRequest, response.dump(),
                  "application/json");
    return;
  }

  // After successful registration, generate authToken for the user
  string userId =
      this->dbManager.checkUserName(to_utf8(username)).getFirst();

  auto authToken = this->tokenHandler.generateToken(userId);

  if (!authToken.empty()) {
    // Registration and authToken generation successful
    response["authToken"] = authToken;
    request.reply(status_codes::OK, response.dump(),
                  "application/json");
  } else {
    // AuthToken generation failed
    response["error"] = "AuthToken generation failed";
    request.reply(status_codes::InternalError, response.dump(),
                  "application/json");
  }
}

void GameServer::handleMessageSendRequest(web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager){
  // Protected route - verify the AuthToken and retrieve the senderId
  auto senderId = verifyAuthToken(request);

  // If senderId is empty, the token is invalid or missing
  if (senderId.empty()) {
    response["error"] = "Invalid or missing AuthToken";
    request.reply(status_codes::Unauthorized, response.dump(),
                  "application/json");
    return; // Stop further processing
  }

  // Extract recipientId and message from request body
  auto recipientId = requestBody[U("recipientId")].as_string();
  auto message = requestBody[U("message")].as_string();

  // Send the message using your backend logic
  bool isSuccessful = this->dbManager.sendMsg(
      senderId, to_utf8(recipientId), to_utf8(message));

  if (isSuccessful) {
    // Message sent successfully
    response["status"] = "Message sent successfully";
    request.reply(status_codes::OK, response.dump(),
                  "application/json");
  } else {
    // Failed to send message
    response["error"] = "Failed to send message";
    request.reply(status_codes::BadRequest, response.dump(),
                  "application/json");
  }
}

void GameServer::handleAddNotificationRequest(web::json::value& requestBody, const string& path, http_request& request, njson response, SessionManager& sessionManager){
  // Protected route - verify the AuthToken and retrieve the userId
  auto userId = verifyAuthToken(request);

  // If userId is empty, the token is invalid or missing
  if (userId.empty()) {
      response["error"] = "Invalid or missing AuthToken";
      request.reply(status_codes::Unauthorized, response.dump(),
                    "application/json");
      return; // Stop further processing
  }

  // Extract the username and the message from the request body
  auto username = requestBody[U("Username")].as_string();
  auto message = requestBody[U("Message")].as_string();

  std::string parsed_message = message.substr(0, 3);
  std::string user_that_added;

  if (parsed_message == "N/A") {
      QueryResult result = dbManager.getUsername(userId);
      user_that_added = result.getFirst();
      message = user_that_added + message.substr(3);
  }

  if (to_utf8(username).empty()) {
      QueryResult query = dbManager.getUsername(to_utf8(userId));
      username = query.getFirst();
  }

  QueryResult query = dbManager.checkUserName(to_utf8(username));

  if (query.isOk()) {
      // Nofitication added successfully
      if (users_notifications[username].size() >= 5) {
          users_notifications.clear();
      }
      users_notifications[username].push_back(message);
      response["status"] = "Notification added successfully";
      request.reply(status_codes::OK, response.dump(),
                    "application/json");
  } else {
      // Failed to add notification
      response["error"] = "Failed to add notification";
      request.reply(status_codes::BadRequest, response.dump(),
                    "application/json");
  }
}
