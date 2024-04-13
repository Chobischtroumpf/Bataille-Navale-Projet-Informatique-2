#include "game_client.hh"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;
using namespace utility::conversions;
using namespace web::json;



GameClient::GameClient(const string& baseUri) {
    try {
        // Convert the base URI from string to utility::string_t
        auto uri = conversions::to_string_t(baseUri);
        // Initialize the http_client with the converted URI
        client = make_unique<http_client>(uri);
        std::clog << "GameClient initialized with URI: " << baseUri <<std::endl;

    } catch (const exception& e) {
        // Catch and print any exceptions that occur during initialization
        std::clog << "Initialization failed: " << e.what() <<std::endl;
    }
}

// Function to test a simple GET request
void GameClient::TestRequest() {
    try {
        // Asynchronously send a GET request to the base URI
        client->request(methods::GET).then([this](http_response response) {
            // Output the status code of the response
            std::clog << "Received response status code: " << response.status_code() <<std::endl;
        }).wait(); // Wait for the request to complete
    } catch (const exception& e) {
        // Catch and print any exceptions that occur during the request
        std::clog << "Test request failed: " << e.what() <<std::endl;
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
            std::clog << "Caught exception: " << e.what() << std::endl;

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
        std::clog << "POST request successful: " << response.dump() << std::endl;
        promise->set_value(); // Indicate success
      })
      .then([promise](pplx::task<void> previousTask) mutable {
        try {
          // This forces the task to execute and any stored exception to be
          // thrown
          previousTask.get();
        } catch (const std::exception &e) {
          // Handle the exception from the POST request
          std::clog << "Caught exception in TestRequest3: " << e.what()
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
          std::clog << "Final catch in TestRequest3: " << e.what() << std::endl;
          // Since the promise might already be set, setting exception here
          // might not always work
          // promise->set_exception(std::current_exception());
        }
      });

  return promise->get_future();
}

future<njson> GameClient::QueryGameState(const string& session_id) {
    std::clog << "Sending GET request to api/games/query" <<std::endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  GetRequest("/api/games/query?session_id=" + session_id)
      .then([promise](njson jsonResponse) {
        // Check if the response contains a 'gameDetails' key
        if (!jsonResponse.empty() && jsonResponse.find("gameDetails") != jsonResponse.end()) {
            // Success path: Extract game details from jsonResponse
            auto gameDetails = jsonResponse["gameDetails"].get<njson>();
            std::clog << "Game info retrieved " <<std::endl;
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
          std::clog << "Exception caught while fetching game state: " << e.what()
               <<std::endl;
          promise->set_value(njson{});
          ; // Indicate failure due to exception
        }
      });

    std::clog << "Game state query request sent." <<std::endl;
    return resultFuture;
}

// Simple function to send a GET request to api/games
future<string> GameClient::GetGames() {
        std::clog << "Sending GET request to api/games" <<std::endl;

        // Use a promise to return the result asynchronously
        auto promise = std::make_shared<std::promise<string>>();
        auto resultFuture = promise->get_future();

        GetRequest("/api/games").then([promise](njson jsonResponse) {
            // Check if the response contains a 'games' key
            if (!jsonResponse.empty() && jsonResponse.find("games") != jsonResponse.end()) {
                // Success path: Extract session Ids from jsonResponse
                auto gameSessions = jsonResponse["games"].get<string>();
                std::clog << "Game sessions retrieved " <<std::endl;
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
                std::clog << "Exception caught while fetching game sessions: " << e.what() <<std::endl;
                promise->set_value(""); // Indicate failure due to exception
            }
        });

    std::clog << "Game retrieve request sent." <<std::endl;
    return resultFuture;
}

// Method to create a new game, taking gameDetails as input and returning a future<string> session_id
future<string> GameClient::CreateGame(const njson& gameDetails) {
    std::clog << "Creating a new game session..." <<std::endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<string>>();
  auto resultFuture = promise->get_future();

  // Send the POST request to the game creation endpoint with gameDetails
  PostRequest("/api/games/create", njson{{"gameDetails", gameDetails}})
      .then([promise](njson jsonResponse) {
        // Check if the response contains a session_id
        if (!jsonResponse.empty() && jsonResponse.find("session_id") != jsonResponse.end()) {
            // Success path: Extract session_id from jsonResponse
            auto session_id = jsonResponse["session_id"].get<string>();
            std::clog << "Game session created with session_id: " << session_id <<std::endl;
            promise->set_value(session_id);
        } else {

            // Error or session_id not provided, set a default error value (empty string)
            std::clog << "No session_id found in jsonResponse" <<std::endl;
            promise->set_value("");
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, indicate failure
          std::clog << "Exception caught while creating game session: " << e.what()
               <<std::endl;
          promise->set_value(""); // Indicate failure due to exception
        }
      });

    std::clog << "Game creation request sent." <<std::endl;
    return resultFuture;
}

// Method to join a game session, returning a future<njson> containing game details
future<njson> GameClient::JoinGame(const string& session_id) {
    std::clog << "Attempting to join game session: " << session_id << " ..." <<std::endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<njson>>();
  auto resultFuture = promise->get_future();

  // Construct the request path with the session_id
  string requestPath = "/api/games/join?session_id=" + session_id;

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
          std::clog << "Exception caught while joining game session: " << e.what() <<std::endl;
          promise->set_value(njson{});
        }
      });

    std::clog << "Join game request sent." <<std::endl;
    return resultFuture;
}

std::future<bool> GameClient::MakeMove(const std::string& session_id, const njson& move) {
    std::clog << "Sending move to game session: " << session_id << " ..." <<std::endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<bool>>();
  auto resultFuture = promise->get_future();

  // Prepare the JSON object with session_id and move
  njson moveData = {{"session_id", session_id}, {"move", move}};

  // Send the POST request to the game move endpoint
  PostRequest("/api/games/move", moveData)
      .then([promise](njson jsonResponse) {
        // Check if the response contains an 'error' key to determine if the
        // move was unsuccessful
        if (jsonResponse.contains("error")) {
          // If an error is present, the move was not successful
          std::clog << "Failed to make a move: "
               << jsonResponse["error"].get<std::string>() <<std::endl;
          promise->set_value(false);
        } else {
            // No error key, assume the move was successful
            std::clog << "Move made successfully." <<std::endl;
            promise->set_value(true);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch and handle any exceptions
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, log it and indicate failure
          std::clog << "Exception caught during make move request: " << e.what()
               <<std::endl;
          promise->set_value(false);
        }
      });

    std::clog << "Move request sent." <<std::endl;
    return resultFuture;
}

// Function to perform login using username and password, returning an authToken asynchronously
future<bool> GameClient::Login(const string& username, const string& password) {
    std::clog << "Initiating login request..." <<std::endl;

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
            std::clog << "Auth Token: " << authToken <<std::endl;

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
          std::clog << "Exception caught in promise chain: " << e.what() <<std::endl;
          promise->set_value(
              ""); // Set value to error signal if it wasn't set before
        }
      });

    std::clog << "Login request sent." <<std::endl;
    return resultFuture;
}

// Function to perform registration using username and password, returning a future<bool> indicating success
future<bool> GameClient::Register(const string& username, const string& password) {
    std::clog << "Initiating registration request..." <<std::endl;

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
            std::clog << "Registration successful. Auth Token: " << authToken <<std::endl;

            // Update the authToken in the GameClient instance
            this->authToken = authToken;
            // Indicate successful registration
            promise->set_value(true);
        } else {
            // authToken not found, indicate registration failure
            std::clog << "Registration failed." <<std::endl;
            promise->set_value(false);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch and handle any exceptions
          errorHandler.get();
        } catch (const exception &e) {
          // In case of exception, log it and indicate failure
          std::clog << "Exception caught during registration: " << e.what() <<std::endl;
          promise->set_value(false);
        }
      });

    std::clog << "Registration request sent." <<std::endl;
    return resultFuture;
}

// Function to get a user ID using the username, returning a future (asynchronous)
future<string> GameClient::GetUserId(const string& username) {
    //std::clog << "Getting userid request for username: " << username << " ..." <<std::endl;

  // Use a promise to return the result asynchronously
  auto promise = std::make_shared<std::promise<string>>();
  auto resultFuture = promise->get_future();

  GetRequest("/api/login/uid?username=" + username)
      .then([promise](njson jsonResponse) {
        // Success path: Process the JSON response here
        if (jsonResponse.contains("userId")) {
            auto userId = jsonResponse["userId"].get<string>();
            //wlogfile << L"User ID: " << wstring(userId.begin(), userId.end()) <<std::endl;
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
          std::clog << "Exception caught in promise chain: " << e.what() <<std::endl;
          promise->set_value(
              ""); // sert value to error signal in case it wasn't set before
        }
      });

    std::clog << "UserId retrieval initiated" <<std::endl;
    return resultFuture;
}

// Function to get a username using the userId, returning a future (asynchronous)
future<string> GameClient::GetUsername(const string& userId) {
    //std::clog << "Getting username request for userId: " << userId << " ..." <<std::endl;
    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<string>>();
    auto resultFuture = promise->get_future();
    GetRequest("/api/username?userId=" + userId)
    .then([promise](njson jsonResponse) {
        // Success path: Process the JSON response here
        if (jsonResponse.contains("username")) {
            auto userId = jsonResponse["username"].get<string>();
            //wlogfile << L"Username: " << wstring(userId.begin(), userId.end()) <<std::endl;
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
          std::clog << "Exception caught in promise chain: " << e.what() <<std::endl;
          promise->set_value(
              ""); // sert value to error signal in case it wasn't set before
        }
      });

  return resultFuture;
}

future<bool> GameClient::SendMessage(const string& recipientId, const string& message) {
    std::clog << "Sending message to user " << recipientId << " ..." <<std::endl;

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
          std::clog << "Failed to send message " <<std::endl;
          promise->set_value(false);
        } else {
            // No error key, assume the request was successful
            std::clog << "Message sent successfully." <<std::endl;
            promise->set_value(true);
        }
      })
      .then([promise](pplx::task<void> catchTask) {
        // Catch and handle exceptions
        try {
          catchTask.get();
        } catch (const exception &e) {
          // In case of exception, indicate failure
          std::clog << "Exception caught while sending message: " << e.what()
               <<std::endl;
          promise->set_value(false); // Indicate failure due to exception
        }
      });

    std::clog << "Message request sent." <<std::endl;
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
                    // std::cout << "Notification added successfully." <<std::endl;
                    promise->set_value(true);
                } else {
                    // If the response indicates failure or the expected success message
                    // is not present
                  std::cout << "Failed to add notification." <<std::endl;
                    promise->set_value(false);
                }
            })
            .then([promise](pplx::task<void> errorHandler) {
                try {
                    // Attempt to catch exceptions if any
                    errorHandler.get();
                } catch (const std::exception &e) {
                    // In case of exception, indicate failure
                    std::clog << "Exception caught while adding notification: " << e.what() <<std::endl;
                    promise->set_value(false); // Indicate failure due to exception
                }
            });

    // std::cout << "Notification add request sent." <<std::endl;
    return resultFuture;
}

future<bool> GameClient::AddFriend(const string& username) {
    //std::clog << "Attempting to add user " << username << " as a friend..." <<std::endl;

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
            //std::clog << "Friend added successfully." <<std::endl;
            promise->set_value(true);
        } else {
            // If the response indicates failure or the expected success message is not present
            std::clog << "Failed to add friend." <<std::endl;
            promise->set_value(false);
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          std::clog << "Exception caught while adding friend: " << e.what() <<std::endl;
          promise->set_value(false); // Indicate failure due to exception
        }
      });

    //std::clog << "Friend add request sent." <<std::endl;
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
                    //cout << "Notifications retrieved" <<std::endl;
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
                    std::clog << "Exception caught while fetching notifications: " << e.what()
                         <<std::endl;
                    promise->set_value(njson{});
                    ; // Indicate failure due to exception
                }
            });

    //cout << "Notifications query request sent." <<std::endl;
    return resultFuture;
}

future<njson> GameClient::GetFriends() {
    //std::clog << "Retrieving user's friend list..." <<std::endl;

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
            //std::clog << "Friend list retrieved successfully." <<std::endl;
            promise->set_value(friendList);
        } else {
            // Error or friend list not provided, set a default error value (empty array)
            std::clog << "Failed to retrieve friend list." <<std::endl;
            promise->set_value(njson::array());
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          std::clog << "Exception caught while retrieving friend list: " << e.what()
               <<std::endl;
          promise->set_value(njson{{"error", e.what()}});
        }
      });

    //std::clog << "Friend list request sent." <<std::endl;
    return resultFuture;
}

future<njson> GameClient::GetMessages(const std::string& recipientId) {
    std::clog << "Fetching conversation with user " << recipientId << "..." <<std::endl;

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
            std::clog << "Conversation retrieved successfully." <<std::endl;
            promise->set_value(conversation);
        } else {
            // Error or conversation not provided, set a default error value (empty object)
            std::clog << "Failed to retrieve conversation." <<std::endl;
            promise->set_value(njson{});
        }
      })
      .then([promise](pplx::task<void> errorHandler) {
        try {
          // Attempt to catch exceptions if any
          errorHandler.get();
        } catch (const std::exception &e) {
          // In case of exception, indicate failure
          std::clog << "Exception caught while fetching conversation: " << e.what()
               <<std::endl;
          promise->set_value(njson{{"error", e.what()}});
        }
      });

    std::clog << "Conversation fetch request sent." <<std::endl;
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
    std::clog << "Preparing to send POST request to path: " << path <<std::endl;

    uri_builder builder(to_string_t(path));
    auto fullUri = builder.to_uri();
    std::clog << "Full URI: " << to_utf8string(fullUri.to_string()) <<std::endl;
    
    http_request request(methods::POST);
    request.set_request_uri(fullUri);
    request.headers().set_content_type(U("application/json"));
    // Convert njson to string for the request body
    request.set_body(data.dump());


    AddAuthHeader(request); // Add the auth header if token is available

    std::clog << "Sending POST request..." <<std::endl;

    // Directly return the task without wrapping it in a try-catch block
    // The try-catch block is unnecessary as the tasks already handle exceptions internally
    return client->request(request).then([](http_response response) -> pplx::task<njson> {
        std::clog << "Received response status code: " << response.status_code() <<std::endl;

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
                            wcerr << L"Server error: " << wstring(error.begin(), error.end()) <<std::endl;
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
            std::string error = e.what();
            std::clog << "RequestError, caught exception: " << error <<std::endl;
            if (error == "Failed to connect to any resolved endpoint") {
              std::clog
                  << "Please do check if you entered the right server address, "
                     "if the server is running and if you have an active "
                     "internet connection. Leaving the application now."
                  <<std::endl;
              exit(1);
            }
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
                        << wstring(error.begin(), error.end()) <<std::endl;
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
            std::string error = e.what();
            std::clog << "RequestError, caught exception: " << error <<std::endl;
            if (error == "Failed to connect to any resolved endpoint") {
              std::clog
                  << "Please do check if you entered the right server address, "
                     "if the server is running and if you have an active "
                     "internet connection. Leaving the application now."
                  <<std::endl;
              exit(1);
            }
            return njson{}; // Return empty object
          }
        }
        return previousTask.get(); // Return object from last task
      });
}


std::string GameClient::getClientUsername() const {
  return _client_username;
}