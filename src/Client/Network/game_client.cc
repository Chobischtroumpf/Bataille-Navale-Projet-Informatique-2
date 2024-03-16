#include "game_client.hh"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;
using namespace utility::conversions;
using namespace web::json;


// Open file stream in append mode
std::ofstream logfile("gameclientlog.txt", std::ios_base::app);

GameClient::GameClient(const string& baseUri) {
    try {
        // Convert the base URI from string to utility::string_t
        auto uri = conversions::to_string_t(baseUri);
        // Initialize the http_client with the converted URI
        client = make_unique<http_client>(uri);
        logfile << "GameClient initialized with URI: " << baseUri << endl;

        // Check if the file stream is open
        if (!logfile.is_open()) {
            std::cerr << "Error: Unable to open log file." << std::endl;
            return;
        }
    } catch (const exception& e) {
        // Catch and print any exceptions that occur during initialization
        cerr << "Initialization failed: " << e.what() << endl;
    }
}

// Function to test a simple GET request
void GameClient::TestRequest() {
    try {
        // Asynchronously send a GET request to the base URI
        client->request(methods::GET).then([this](http_response response) {
            // Output the status code of the response
            logfile << "Received response status code: " << response.status_code() << endl;
        }).wait(); // Wait for the request to complete
    } catch (const exception& e) {
        // Catch and print any exceptions that occur during the request
        cerr << "Test request failed: " << e.what() << endl;
    }
}

std::future<void> GameClient::TestRequest2() {
  auto promise = std::make_shared<std::promise<void>>();

  client->request(methods::GET)
      .then([promise](web::http::http_response response) mutable {
        promise->set_value(); // Indicate success
      })
      .then([promise](pplx::task<void> previous_task) mutable {
        if (previous_task._GetImpl()->_HasUserException()) {
          auto holder = previous_task._GetImpl()->_GetExceptionHolder();
          try {
            holder->_RethrowUserException();
          } catch (std::exception &e) {
            // Handle the exception
            std::cerr << "Caught exception: " << e.what() << std::endl;

            promise->set_value(); // Indicate success
          }
        }
      });

  return promise->get_future();
}

std::future<void> GameClient::TestRequest3() {
  auto promise = std::make_shared<std::promise<void>>();
  njson data;

  // Call PostRequest with some endpoint and data
  PostRequest("/test/endpoint", data)
      .then([promise](njson response) {
        // Handle the successful response here
        logfile << "POST request successful: " << response.dump() << std::endl;
        promise->set_value(); // Indicate success
      })
      .then([promise](pplx::task<void> previousTask) mutable {
        try {
          // This forces the task to execute and any stored exception to be
          // thrown
          previousTask.get();
        } catch (const std::exception &e) {
          // Handle the exception from the POST request
          std::cerr << "Caught exception in TestRequest3: " << e.what()
                    << std::endl;
          promise->set_exception(
              std::current_exception()); // Pass the exception to the future
        }
      })
      .then([=](pplx::task<void> task) mutable {
        try {
          // Attempt to get the result to see if any exceptions were stored
          task.get(); // If an exception was stored, it will be thrown here
        } catch (const std::exception &e) {
          std::cerr << "Final catch in TestRequest3: " << e.what() << std::endl;
          // Since the promise might already be set, setting exception here
          // might not always work
          // promise->set_exception(std::current_exception());
        }
      });

  return promise->get_future();
}

future<njson> GameClient::QueryGameState(const string& sessionId) {
    logfile << "Sending GET request to api/games/query" << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  GetRequest("/api/games/query?sessionId=" + sessionId)
      .then([promise](njson jsonResponse) {
        // Check if the response contains a 'gameDetails' key
        if (!jsonResponse.empty() && jsonResponse.find("gameDetails") != jsonResponse.end()) {
            // Success path: Extract game details from jsonResponse
            auto gameDetails = jsonResponse["gameDetails"].get<njson>();
            logfile << "Game info retrieved " << endl;
            promise->set_value(gameDetails);
        } else {
          // Error or info not provided, set a default error value (empty
          // object)
          promise->set_value(njson{});
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while fetching game state: " << e.what()
               << endl;
          promise->set_value(njson{});
          ; // Indicate failure due to exception
        }
      });

    logfile << "Game state query request sent." << endl;
    return resultFuture;
}

// Simple function to send a GET request to api/games
future<string> GameClient::GetGames() {
        logfile << "Sending GET request to api/games" << endl;

        // Use a promise to return the result asynchronously
        auto promise = std::make_shared<std::promise<string>>();
        auto resultFuture = promise->get_future();

        GetRequest("/api/games").then([promise](njson jsonResponse) {
            // Check if the response contains a 'games' key
            if (!jsonResponse.empty() && jsonResponse.find("games") != jsonResponse.end()) {
                // Success path: Extract session Ids from jsonResponse
                auto gameSessions = jsonResponse["games"].get<string>();
                logfile << "Game sessions retrieved " << endl;
                promise->set_value(gameSessions);
            } else {
                // Error or sessions not provided, set a default error value (empty string)
                promise->set_value("");
            }
        }).then([promise](pplx::task<void> errorHandler) {
            try {
                // Attempt to catch exceptions if any
                errorHandler.get();
            } catch (const exception& e) {
                // In case of exception, indicate failure
                cerr << "Exception caught while fetching game sessions: " << e.what() << endl;
                promise->set_value(""); // Indicate failure due to exception
            }
        });

    logfile << "Game retrieve request sent." << endl;
    return resultFuture;
}

// Method to create a new game, taking gameDetails as input and returning a future<string> sessionId
future<string> GameClient::CreateGame(const njson& gameDetails) {
    logfile << "Creating a new game session..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<string>>();
  auto resultFuture = promise->get_future();

  // Send the POST request to the game creation endpoint with gameDetails
  PostRequest("/api/games/create", njson{{"gameDetails", gameDetails}})
      .then([promise](njson jsonResponse) {
        // Check if the response contains a sessionId
        if (!jsonResponse.empty() && jsonResponse.find("sessionId") != jsonResponse.end()) {
            // Success path: Extract sessionId from jsonResponse
            auto sessionId = jsonResponse["sessionId"].get<string>();
            logfile << "Game session created with sessionId: " << sessionId << endl;
            promise->set_value(sessionId);
        } else {

            // Error or sessionId not provided, set a default error value (empty string)
            logfile << "No sessionId found in jsonResponse" << endl;
            promise->set_value("");
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while creating game session: " << e.what()
               << endl;
          promise->set_value(""); // Indicate failure due to exception
        }
      });

    logfile << "Game creation request sent." << endl;
    return resultFuture;
}

// Method to join a game session, returning a future<njson> containing game details
future<njson> GameClient::JoinGame(const string& sessionId) {
    logfile << "Attempting to join game session: " << sessionId << " ..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  // Construct the request path with the sessionId
  string requestPath = "/api/games/join?sessionId=" + sessionId;

  // Make the GET request to join the game and retrieve the session details
  GetRequest(requestPath)
      .then([promise](njson jsonResponse) {
        // Check if the response contains game details
        if (!jsonResponse.empty() &&
            jsonResponse.find("error") == jsonResponse.end()) {
          // Success path: jsonResponse contains game details
          promise->set_value(jsonResponse);
        } else {
          // Error or game session not found, return empty object
          promise->set_value(njson{});
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, set an error value
          cerr << "Exception caught while joining game session: " << e.what()
               << endl;
          promise->set_value(njson{});
        }
      });

    logfile << "Join game request sent." << endl;
    return resultFuture;
}

std::future<bool> GameClient::MakeMove(const std::string& sessionId, const njson& move) {
    logfile << "Sending move to game session: " << sessionId << " ..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();

  // Prepare the JSON object with sessionId and move
  njson moveData = {{"sessionId", sessionId}, {"move", move}};

  // Send the POST request to the game move endpoint
  PostRequest("/api/games/move", moveData.dump())
      .then([promise](njson jsonResponse) {
        // Check if the response contains an 'error' key to determine if the
        // move was unsuccessful
        if (jsonResponse.contains("error")) {
          // If an error is present, the move was not successful
          cerr << "Failed to make a move: "
               << jsonResponse["error"].get<std::string>() << endl;
          promise->set_value(false);
        } else {
            // No error key, assume the move was successful
            logfile << "Move made successfully." << endl;
            promise->set_value(true);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch and handle any exceptions
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, log it and indicate failure
          cerr << "Exception caught during make move request: " << e.what()
               << endl;
          promise->set_value(false);
        }
      });

    logfile << "Move request sent." << endl;
    return promise->get_future();
}

// Function to perform login using username and password, returning an authToken asynchronously
future<bool> GameClient::Login(const string& username, const string& password) {
    logfile << "Initiating login request..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();
  auto resultFuture = promise->get_future();

  // Prepare the JSON object with username and password
  njson loginData;
  loginData["username"] = username;
  loginData["password"] = password;
  this->_client_username = username;

  // Send the POST request to the login endpoint
  PostRequest("/api/login", loginData)
      .then([promise, this](njson jsonResponse) {
        // Process the JSON response here
        if (jsonResponse.contains("authToken")) {

            auto authToken = jsonResponse["authToken"].get<string>();
            logfile << "Auth Token: " << authToken << endl;

          this->authToken = authToken;
          promise->set_value(true);
        } else {
          // Error or authToken not found, set a default error value (empty
          // string)
          promise->set_value(false);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, set an error value
          cerr << "Exception caught in promise chain: " << e.what() << endl;
          promise->set_value(
              ""); // Set value to error signal if it wasn't set before
        }
      });

    logfile << "Login request sent." << endl;
    return resultFuture;
}

// Function to perform registration using username and password, returning a future<bool> indicating success
future<bool> GameClient::Register(const string& username, const string& password) {
    logfile << "Initiating registration request..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();
  auto resultFuture = promise->get_future();

  // Prepare the JSON object with username and password
  njson registerData;
  registerData["username"] = username;
  registerData["password"] = password;

  // Send the POST request to the registration endpoint
  PostRequest("/api/register", registerData)
      .then([promise, this](njson jsonResponse) {
        // Process the JSON response here
        if (jsonResponse.contains("authToken")) {
            // Extract the authToken from the response
            auto authToken = jsonResponse["authToken"].get<string>();
            logfile << "Registration successful. Auth Token: " << authToken << endl;

            // Update the authToken in the GameClient instance
            this->authToken = authToken;
            // Indicate successful registration
            promise->set_value(true);
        } else {
            // authToken not found, indicate registration failure
            logfile << "Registration failed." << endl;
            promise->set_value(false);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch and handle any exceptions
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, log it and indicate failure
          cerr << "Exception caught during registration: " << e.what() << endl;
          promise->set_value(false);
        }
      });

    logfile << "Registration request sent." << endl;
    return resultFuture;
}

// Function to get a user ID using the username, returning a future (asynchronous)
future<string> GameClient::GetUserId(const string& username) {
    //logfile << "Getting userid request for username: " << username << " ..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<string>>();
  auto resultFuture = promise->get_future();

  GetRequest("/api/login/uid?username=" + username)
      .then([promise](njson jsonResponse) {
        // Success path: Process the JSON response here
        if (jsonResponse.contains("userId")) {
            auto userId = jsonResponse["userId"].get<string>();
            //wlogfile << L"User ID: " << wstring(userId.begin(), userId.end()) << endl;
            promise->set_value(userId);
        } else {
          // Error or userId not found, set a default error value (empty string)
          promise->set_value("");
        }
      })
      .then([promise](pplx::task<void> catchTask) {
        try {
          // Attempt to catch exceptions if any
          catchTask.get();
        } catch (const exception &e) {
          // In case of exception, set an error value
          cerr << "Exception caught in promise chain: " << e.what() << endl;
          promise->set_value(
              ""); // sert value to error signal in case it wasn't set before
        }
      });

    logfile << "Login request initiated." << endl;
    return resultFuture;
}

// Function to get a username using the userId, returning a future (asynchronous)
future<string> GameClient::GetUsername(const string& userId) {
    //logfile << "Getting username request for userId: " << userId << " ..." << endl;
    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<string>>();
    auto resultFuture = promise->get_future();
    GetRequest("/api/username?userId=" + userId)
    .then([promise](njson jsonResponse) {
        // Success path: Process the JSON response here
        if (jsonResponse.contains("username")) {
            auto userId = jsonResponse["username"].get<string>();
            //wlogfile << L"Username: " << wstring(userId.begin(), userId.end()) << endl;
            promise->set_value(userId);
        } else {
          // Error or userId not found, set a default error value (empty string)
          promise->set_value("");
        }
      })
      .then([promise](pplx::task<void> catchTask) {
        try {
          // Attempt to catch exceptions if any
          catchTask.get();
        } catch (const exception &e) {
          // In case of exception, set an error value
          cerr << "Exception caught in promise chain: " << e.what() << endl;
          promise->set_value(
              ""); // sert value to error signal in case it wasn't set before
        }
      });

  return resultFuture;
}

future<bool> GameClient::SendMessage(const string& recipientId, const string& message) {
    logfile << "Sending message to user " << recipientId << " ..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();

  // Prepare the JSON object with data
  njson messageData = {{"recipientId", recipientId}, {"message", message}};

  // Send the POST request to the message endpoint
  PostRequest("/api/chat/send", messageData)
      .then([promise](njson response) {
        // Check if the response contains an 'error' key
        if (response.contains("error")) {
          // If an error key exists, the request was not successful
          cerr << "Failed to send message " << endl;
          promise->set_value(false);
        } else {
            // No error key, assume the request was successful
            logfile << "Message sent successfully." << endl;
            promise->set_value(true);
        }
      })
      .then([promise](pplx::task<void> catchTask) {
        // Catch and handle exceptions
        try {
          catchTask.get();
        } catch (const exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while sending message: " << e.what()
               << endl;
          promise->set_value(false); // Indicate failure due to exception
        }
      });

    logfile << "Message request sent." << endl;
    return promise->get_future();
}

future<bool> GameClient::AddNotification(const std::string &username, const std::string &message) {

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<bool>>();
    auto resultFuture = promise->get_future();

    // Prepare the JSON object with data
    njson friendData = {{"Username", username }, {"Message", message}};

    // Send the POST request to the add notification endpoint
    PostRequest("/api/user/add/notification", friendData)
            .then([promise](njson response) {
                // Check the response to see if adding the friend was successful
                if (!response.contains("error")) {
                    // cout << "Notification added successfully." << endl;
                    promise->set_value(true);
                } else {
                    // If the response indicates failure or the expected success message
                    // is not present
                    cout << "Failed to add notification." << endl;
                    promise->set_value(false);
                }
            })
            .then([promise](pplx::task<void> errorHandler) {
                try {
                    // Attempt to catch exceptions if any
                    errorHandler.get();
                } catch (const std::exception &e) {
                    // In case of exception, indicate failure
                    cerr << "Exception caught while adding notification: " << e.what() << endl;
                    promise->set_value(false); // Indicate failure due to exception
                }
            });

    // cout << "Notification add request sent." << endl;
    return resultFuture;
}

future<bool> GameClient::AddFriend(const string& username) {
    //logfile << "Attempting to add user " << username << " as a friend..." << endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();
  auto resultFuture = promise->get_future();

  // Prepare the JSON object with data
  njson friendData = {{"friendUsername", username}};

  // Send the POST request to the add friend endpoint
  PostRequest("/api/friend/add", friendData)
      .then([promise](njson response) {
        // Check the response to see if adding the friend was successful
        if (!response.contains("error")) {
            //logfile << "Friend added successfully." << endl;
            promise->set_value(true);
        } else {
            // If the response indicates failure or the expected success message is not present
            logfile << "Failed to add friend." << endl;
            promise->set_value(false);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while adding friend: " << e.what() << endl;
          promise->set_value(false); // Indicate failure due to exception
        }
      });

    //logfile << "Friend add request sent." << endl;
    return resultFuture;
}

future<njson> GameClient::getNotificationsFromServer() {

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<njson>>();
    auto resultFuture = promise->get_future();

    GetRequest("/api/notification/get")
            .then([promise](njson jsonResponse) {
                // Check if the response contains a 'gameDetails' key
                if (!jsonResponse.empty() &&
                    jsonResponse.find("notifications") != jsonResponse.end()) {
                    // Success path: Extract game details from jsonResponse
                    auto Notifications = jsonResponse["notifications"].get<njson>();
                    //cout << "Notifications retrieved" << endl;
                    promise->set_value(Notifications);
                } else {
                    // Error or info not provided, set a default error value (empty
                    // object)
                    promise->set_value(njson{});
                }
            })
            .then([promise](pplx::task<void> errorHandler) {
                try {
                    // Attempt to catch exceptions if any
                    errorHandler.get();
                } catch (const exception &e) {
                    // In case of exception, indicate failure
                    cerr << "Exception caught while fetching notifications: " << e.what()
                         << endl;
                    promise->set_value(njson{});
                    ; // Indicate failure due to exception
                }
            });

    //cout << "Notifications query request sent." << endl;
    return resultFuture;
}

future<njson> GameClient::GetFriends() {
    //logfile << "Retrieving user's friend list..." << endl;

  // Use a shared promise to return the friend list asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  // Perform the GET request to the friend list endpoint
  GetRequest("/api/friend/list")
      .then([promise](njson jsonResponse) {
        // Check if the response contains a 'friends' key
        if (!jsonResponse.empty() && jsonResponse.find("friends") != jsonResponse.end()) {
            // Success path: Extract friends list from jsonResponse
            auto friendList = jsonResponse["friends"];
            //logfile << "Friend list retrieved successfully." << endl;
            promise->set_value(friendList);
        } else {
            // Error or friend list not provided, set a default error value (empty array)
            logfile << "Failed to retrieve friend list." << endl;
            promise->set_value(njson::array());
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while retrieving friend list: " << e.what()
               << endl;
          promise->set_value(njson{{"error", e.what()}});
        }
      });

    //logfile << "Friend list request sent." << endl;
    return resultFuture;
}

future<njson> GameClient::GetMessages(const std::string& recipientId) {
    logfile << "Fetching conversation with user " << recipientId << "..." << endl;

  // Use a shared promise to return the conversation asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  // Construct the query URL with recipientId as a query parameter
  std::string queryUri = "/api/chat/get?recipientId=" + recipientId;

  // Perform the GET request
  GetRequest(queryUri)
      .then([promise](njson jsonResponse) {
        // Check if the response contains a 'conversation' key
        if (!jsonResponse.empty() && jsonResponse.find("conversation") != jsonResponse.end()) {
            // Success path: Extract conversation from jsonResponse
            auto conversation = jsonResponse["conversation"];
            logfile << "Conversation retrieved successfully." << endl;
            promise->set_value(conversation);
        } else {
            // Error or conversation not provided, set a default error value (empty object)
            logfile << "Failed to retrieve conversation." << endl;
            promise->set_value(njson{});
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          cerr << "Exception caught while fetching conversation: " << e.what()
               << endl;
          promise->set_value(njson{{"error", e.what()}});
        }
      });

    logfile << "Conversation fetch request sent." << endl;
    return resultFuture;
}

// Add Authorization header to requests if authToken is available
void GameClient::AddAuthHeader(http_request &request) {
  if (!authToken.empty()) {
    request.headers().add(U("Authorization"),
                          U("Bearer ") + conversions::to_string_t(authToken));
  }
}

// General-purpose POST request handler
pplx::task<njson> GameClient::PostRequest(const string& path, const njson& data) {
    logfile << "Preparing to send POST request to path: " << path << endl;

    uri_builder builder(to_string_t(path));
    auto fullUri = builder.to_uri();
    logfile << "Full URI: " << to_utf8string(fullUri.to_string()) << endl;
    
    http_request request(methods::POST);
    request.set_request_uri(fullUri);
    request.headers().set_content_type(U("application/json"));
    // Convert njson to string for the request body
    request.set_body(data.dump());


    AddAuthHeader(request); // Add the auth header if token is available

    logfile << "Sending POST request..." << endl;

    // Directly return the task without wrapping it in a try-catch block
    // The try-catch block is unnecessary as the tasks already handle exceptions internally
    return client->request(request).then([](http_response response) -> pplx::task<njson> {
        logfile << "Received response status code: " << response.status_code() << endl;

       if (response.status_code() == status_codes::OK || response.status_code() == status_codes::Created) {
                // Convert and return the web::json::value to njson
                return response.extract_json().then([](web::json::value webJsonValue) -> njson {
                    return njson::parse(webJsonValue.serialize());
                });
            } else {
                // Handle non-OK responses
                return response.extract_json().then([&response](web::json::value webJsonValue) -> njson {
                        // Check if json has an error field sent from server
                        auto errorJson = njson::parse(webJsonValue.serialize());
                        // Check if the njson has an "error" field
                        if (errorJson.contains("error")) {
                            // Extract the error message as a string
                            auto error = errorJson["error"].get<string>();
                            // Output the error message
                            wcerr << L"Server error: " << wstring(error.begin(), error.end()) << endl;
                            // Throw runtime error
                            throw runtime_error("Server error: " + errorJson.dump());
                        }
                        // Else throw http exception
                        throw http_exception(response.status_code());
                });
            }
            //Catch errors at the request level
    }).then([=](pplx::task<njson> previousTask) mutable {
        
        // Handle request level errors
        if (previousTask._GetImpl()->_HasUserException()) {
          auto holder = previousTask._GetImpl()->_GetExceptionHolder();
          try {
            holder->_RethrowUserException();
          } catch (std::exception &e) {
            cerr << "RequestError, caught exception: " << e.what() << endl;
            return njson{}; // return empty object
          }
        }
        return previousTask.get(); // Return object from last task
      });
}

// General-purpose GET request handler
pplx::task<njson> GameClient::GetRequest(const string &path) {

  uri_builder builder(to_string_t(path));
  auto requestUri = builder.to_uri();

  http_request request(methods::GET);
  request.set_request_uri(requestUri);
  AddAuthHeader(request); // Add the auth header if token is available

  return client->request(request)
      .then([](http_response response) -> pplx::task<njson> {
        if (response.status_code() == status_codes::OK ||
            response.status_code() == status_codes::Created) {
          // Convert and return the web::json::value to njson
          return response.extract_json().then(
              [](web::json::value webJsonValue) -> njson {
                return njson::parse(webJsonValue.serialize());
              });
        } else {
          // Handle non-OK responses
          return response.extract_json().then(
              [&response](web::json::value webJsonValue) -> njson {
                // Check if json has an error field sent from server
                auto errorJson = njson::parse(webJsonValue.serialize());
                // Check if the njson has an "error" field
                if (errorJson.contains("error")) {
                  // Extract the error message as a string
                  auto error = errorJson["error"].get<string>();
                  // Output the error message
                  wcerr << L"Server error: "
                        << wstring(error.begin(), error.end()) << endl;
                  // Throw runtime error
                  throw runtime_error("Server error: " + errorJson.dump());
                }
                // Else throw http exception
                throw http_exception(response.status_code());
              });
        }
      })
      .then([=](pplx::task<njson> previousTask) mutable {
        // Handle request level errors
        if (previousTask._GetImpl()->_HasUserException()) {
          auto holder = previousTask._GetImpl()->_GetExceptionHolder();
          try {
            holder->_RethrowUserException();
          } catch (std::exception &e) {
            cerr << "RequestError, caught exception: " << e.what() << endl;
            return njson{}; // Return empty object
          }
        }
        return previousTask.get(); // Return object from last task
      });
}


std::string GameClient::getClientUsername() const {
  return _client_username;
}