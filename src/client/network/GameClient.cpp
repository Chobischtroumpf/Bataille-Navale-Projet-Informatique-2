#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory> // For unique_ptr
#include <future> // For future
#include "GameClient.hpp" 

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;
using namespace utility::conversions;
using namespace web::json;
using njson = nlohmann::json;

GameClient::GameClient(const string& baseUri) {
    try {
        // Convert the base URI from string to utility::string_t
        auto uri = conversions::to_string_t(baseUri);
        // Initialize the http_client with the converted URI
        client = make_unique<http_client>(uri);
        cout << "GameClient initialized with URI: " << baseUri << endl;
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
            wcout << L"Received response status code: " << response.status_code() << endl;
        }).wait(); // Wait for the request to complete
    } catch (const exception& e) {
        // Catch and print any exceptions that occur during the request
        cerr << "Test request failed: " << e.what() << endl;
    }
}

std::future<void> GameClient::TestRequest2() {
    auto promise = std::make_shared<std::promise<void>>();

    client->request(methods::GET).then([promise] (web::http::http_response response) mutable {
        promise->set_value(); // Indicate success
    }).then([promise] (pplx::task<void> previous_task) mutable {
        if (previous_task._GetImpl()->_HasUserException()) {
            auto holder = previous_task._GetImpl()->_GetExceptionHolder();
            try {
                holder->_RethrowUserException();
            } catch (std::exception& e) {
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
    PostRequest("/test/endpoint", data).then([promise](njson response) {
        // Handle the successful response here
        std::cout << "POST request successful: " << response.dump() << std::endl;
        promise->set_value(); // Indicate success
    }).then([promise](pplx::task<void> previousTask) mutable {
        try {
            // This forces the task to execute and any stored exception to be thrown
            previousTask.get();
        } catch (const std::exception& e) {
            // Handle the exception from the POST request
            std::cerr << "Caught exception in TestRequest3: " << e.what() << std::endl;
            promise->set_exception(std::current_exception()); // Pass the exception to the future
        }
    }).then([=](pplx::task<void> task) mutable {
        try {
            // Attempt to get the result to see if any exceptions were stored
            task.get(); // If an exception was stored, it will be thrown here
        } catch (const std::exception& e) {
            std::cerr << "Final catch in TestRequest3: " << e.what() << std::endl;
            // Since the promise might already be set, setting exception here might not always work
            // promise->set_exception(std::current_exception()); 
        }
    });

    return promise->get_future();
}

future<njson> GameClient::QueryGameState(const string& sessionId, const string& userId) {
    cout << "Sending GET request to api/games..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<njson>>();
    auto resultFuture = promise->get_future();

    GetRequest("/api/games/query?sessionid=" + sessionId + "&userid=" + userId ).then([promise](njson jsonResponse) {
        // Check if the response contains a 'gameDetails' key
        if (!jsonResponse.empty() && jsonResponse.find("gameDetails") != jsonResponse.end()) {
            // Success path: Extract game details from jsonResponse
            auto gameDetails = jsonResponse["gameDetails"].get<njson>();
            cout << "Game info retrieved " << endl;
            promise->set_value(gameDetails);
        } else {
            // Error or info not provided, set a default error value (empty object)
            promise->set_value(njson {});
        }
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, indicate failure
            cerr << "Exception caught while fetching game state: " << e.what() << endl;
            promise->set_value(njson {});; // Indicate failure due to exception
        }
    });

    cout << "Game creation request sent." << endl;
    return resultFuture;
}

// Simple function to send a GET request to api/games
future<string> GameClient::GetGames() {
        cout << "Sending GET request to api/games..." << endl;

        // Use a promise to return the result asynchronously
        auto promise = std::make_shared<std::promise<string>>();
        auto resultFuture = promise->get_future();

        GetRequest("/api/games").then([promise](njson jsonResponse) {
            // Check if the response contains a 'games' key
            if (!jsonResponse.empty() && jsonResponse.find("games") != jsonResponse.end()) {
                // Success path: Extract session Ids from jsonResponse
                auto gameSessions = jsonResponse["games"].get<string>();
                cout << "Game sessions retrieved " << endl;
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

    cout << "Game creation request sent." << endl;
    return resultFuture;
}

// Method to create a new game, taking gameDetails as input and returning a future<string> sessionId
future<string> GameClient::CreateGame(const njson& gameDetails) {
    cout << "Creating a new game session..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<string>>();
    auto resultFuture = promise->get_future();

    // Send the POST request to the game creation endpoint with gameDetails
    PostRequest("/api/game/create", gameDetails)
    .then([promise](njson jsonResponse) {
        // Check if the response contains a sessionId
        if (!jsonResponse.empty() && jsonResponse.find("sessionId") != jsonResponse.end()) {
            // Success path: Extract sessionId from jsonResponse
            auto sessionId = jsonResponse["sessionId"].get<string>();
            cout << "Game session created with sessionId: " << sessionId << endl;
            promise->set_value(sessionId);
        } else {

            // Error or sessionId not provided, set a default error value (empty string)
            cout << "No sessionId found in jsonResponse" << endl;
            promise->set_value("");
        }
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, indicate failure
            cerr << "Exception caught while creating game session: " << e.what() << endl;
            promise->set_value(""); // Indicate failure due to exception
        }
    });

    cout << "Game creation request sent." << endl;
    return resultFuture;
}

// Method to join a game session, returning a future<njson> containing game details
future<njson> GameClient::JoinGame(const string& sessionId) {
    cout << "Attempting to join game session: " << sessionId << " ..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<njson>>();
    auto resultFuture = promise->get_future();

    // Construct the request path with the sessionId
    string requestPath = "/api/game/join?sessionId="  + sessionId;

    // Make the GET request to join the game and retrieve the session details
    GetRequest(requestPath)
    .then([promise](njson jsonResponse) {
        // Check if the response contains game details
        if (!jsonResponse.empty() && jsonResponse.find("error") == jsonResponse.end()) {
            // Success path: jsonResponse contains game details
            promise->set_value(jsonResponse);
        } else {
            // Error or game session not found, pass the received object 
            promise->set_value(jsonResponse);
        }
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, set an error value
            cerr << "Exception caught while joining game session: " << e.what() << endl;
            promise->set_value(njson{});
        }
    });

    cout << "Join game request sent." << endl;
    return resultFuture;
}

// Method to make a move in a game session, returning a future<bool> indicating success
future<bool> GameClient::MakeMove(const string& sessionId, const string& move) {
    cout << "Sending move to game session: " << sessionId << " ..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<bool>>();
    auto resultFuture = promise->get_future();

    // Prepare the JSON object with sessionId and move
    njson moveData;
    moveData["sessionId"] = sessionId;
    moveData["move"] = move;

    // Serialize nlohmann::json object to string for the POST request
    string serializedData = moveData.dump();

    // Send the POST request to the game move endpoint
    PostRequest("/api/game/move", serializedData)
    .then([promise](bool success) {
        // Directly use the boolean result of the PostRequest to fulfill the promise
        promise->set_value(success);
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, indicate failure
            cerr << "Exception caught while making a move: " << e.what() << endl;
            promise->set_value(false); // Indicate failure due to exception
        }
    });

    cout << "Move request sent." << endl;
    return resultFuture;
}

// Function to perform login using userId and password, returning an authToken asynchronously
future<string> GameClient::Login(const string& userId, const string& password) {
    cout << "Initiating login request..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<string>>();
    auto resultFuture = promise->get_future();

    // Prepare the JSON object with userId and password
    njson loginData;
    loginData["userId"] = userId;  
    loginData["password"] = password;

    // Send the POST request to the login endpoint
    PostRequest("/api/login", loginData)
    .then([promise](njson jsonResponse) {

        // Process the JSON response here
        if (jsonResponse.contains("authToken")) {

            auto authToken = jsonResponse["authToken"].get<string>();
            cout << "Auth Token: " << authToken << endl;
            promise->set_value(to_utf8string(authToken));
        } else {
            // Error or authToken not found, set a default error value (empty string)
            promise->set_value("");
        }
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, set an error value
            cerr << "Exception caught in promise chain: " << e.what() << endl;
            promise->set_value(""); // Set value to error signal if it wasn't set before
        }
    });

    cout << "Login request sent." << endl;
    return resultFuture;
}

// Function to get a user ID using the username, returning a future (asynchronous)
future<string> GameClient::GetUserId(const string& username) {
    cout << "Sending login request for username: " << username << " ..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<string>>();
    auto resultFuture = promise->get_future();

    GetRequest("/api/login/uid?username=" + username)
    .then([promise](njson jsonResponse) {
        // Success path: Process the JSON response here
        if (jsonResponse.contains("userId")) {
            auto userId = jsonResponse["userId"].get<string>();
            wcout << L"User ID: " << wstring(userId.begin(), userId.end()) << endl;
            promise->set_value(userId);
        } else {
            // Error or userId not found, set a default error value (empty string)
            promise->set_value("");
        }
    }).then([promise](pplx::task<void> catchTask) {
        try {
            // Attempt to catch exceptions if any
            catchTask.get();
        } catch (const exception& e) {
            // In case of exception, set an error value
            cerr << "Exception caught in promise chain: " << e.what() << endl;
            promise->set_value(""); // sert value to error signal in case it wasn't set before
        }
    });

    cout << "Login request initiated." << endl;
    return resultFuture;
}

// Method to make a move in a game session, returning a future<bool> indicating success
future<bool> GameClient::SendMessage(const string& senderId, const string& recipientId, const string& message) {
    cout << "Sending message to user " << recipientId << " ..." << endl;

    // Use a promise to return the result asynchronously
    auto promise = std::make_shared<std::promise<bool>>();
    auto resultFuture = promise->get_future();

    // Prepare the JSON object with data 
    njson moveData;
    moveData["recipientId"] = recipientId;
    moveData["message"] = message;

    // Serialize nlohmann::json object to string for the POST request
    string serializedData = moveData.dump();

    // Send the POST request to the game move endpoint
    PostRequest("/api/chat/send", serializedData)
    .then([promise](bool success) {
        // Directly use the boolean result of the PostRequest to fulfill the promise
        promise->set_value(success);
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const exception& e) {
            // In case of exception, indicate failure
            cerr << "Exception caught while sending message: " << e.what() << endl;
            promise->set_value(false); // Indicate failure due to exception
        }
    });

    cout << "Move request sent." << endl;
    return resultFuture;
}

future<nlohmann::json> GameClient::GetMessages(const std::string& recipientId) {
    cout << "Fetching conversation with user " << recipientId << "..." << endl;

    // Use a shared promise to return the conversation asynchronously
    auto promise = std::make_shared<std::promise<nlohmann::json>>();
    auto resultFuture = promise->get_future();

    // Construct the query URL with recipientId as a query parameter
    std::string queryUri = "/api/chat/get?recipientId=" + recipientId ;

    // Perform the GET request
    GetRequest(queryUri)
    .then([promise](nlohmann::json jsonResponse) {
        // Check if the response contains a 'conversation' key
        if (!jsonResponse.empty() && jsonResponse.find("conversation") != jsonResponse.end()) {
            // Success path: Extract conversation from jsonResponse
            auto conversation = jsonResponse["conversation"];
            cout << "Conversation retrieved successfully." << endl;
            promise->set_value(conversation);
        } else {
            // Error or conversation not provided, set a default error value (empty object)
            cout << "Failed to retrieve conversation." << endl;
            promise->set_value(nlohmann::json{});
        }
    }).then([promise](pplx::task<void> errorHandler) {
        try {
            // Attempt to catch exceptions if any
            errorHandler.get();
        } catch (const std::exception& e) {
            // In case of exception, indicate failure
            cerr << "Exception caught while fetching conversation: " << e.what() << endl;
            promise->set_value(nlohmann::json{{"error", e.what()}});
        }
    });

    cout << "Conversation fetch request sent." << endl;
    return resultFuture;
}

// General-purpose POST request handler
pplx::task<njson> GameClient::PostRequest(const string& path, const njson& data) {
    cout << "Preparing to send POST request to path: " << path << endl;

    uri_builder builder(to_string_t(path));
    auto fullUri = builder.to_uri();
    cout << "Full URI: " << to_utf8string(fullUri.to_string()) << endl;

    http_request request(methods::POST);
    request.headers().set_content_type(U("application/json"));
    // Convert nlohmann::json to string for the request body
    request.set_body(data.dump());

    cout << "Sending POST request..." << endl;

    // Directly return the task without wrapping it in a try-catch block
    // The try-catch block is unnecessary as the tasks already handle exceptions internally
    return client->request(request).then([](http_response response) -> pplx::task<njson> {
        cout << "Received response status code: " << response.status_code() << endl;

       if (response.status_code() == status_codes::OK || response.status_code() == status_codes::Created) {
                // Convert and return the web::json::value to nlohmann::json
                return response.extract_json().then([](web::json::value webJsonValue) -> njson {
                    return njson::parse(webJsonValue.serialize());
                });
            } else {
                // Handle non-OK responses
                return response.extract_json().then([&response](web::json::value webJsonValue) -> njson {
                        // Check if json has an error field sent from server
                        auto errorJson = njson::parse(webJsonValue.serialize());
                        // Check if the nlohmann::json has an "error" field
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
            } catch (std::exception& e) {
                cerr << "RequestError, caught exception: " << e.what() << endl;
                return njson{}; //return empty object
            }
        }
        return previousTask.get(); // Return object from last task
    });
}

// General-purpose GET request handler
pplx::task<njson> GameClient::GetRequest(const string& path) {
    
    uri_builder builder(to_string_t(path));
    auto requestUri = builder.to_uri().to_string();
    http_request request(methods::GET);

    return client->request(request).then([](http_response response) -> pplx::task<njson> {
        if (response.status_code() == status_codes::OK || response.status_code() == status_codes::Created) {
            // Convert and return the web::json::value to nlohmann::json
            return response.extract_json().then([](web::json::value webJsonValue) -> njson {
                return njson::parse(webJsonValue.serialize());
            });
        } else {
            // Handle non-OK responses
            return response.extract_json().then([&response](web::json::value webJsonValue) -> njson {
                    // Check if json has an error field sent from server
                    auto errorJson = njson::parse(webJsonValue.serialize());
                    // Check if the nlohmann::json has an "error" field
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
            }).then([=](pplx::task<njson> previousTask) mutable {
                
                // Handle request level errors
                if (previousTask._GetImpl()->_HasUserException()) {
                    auto holder = previousTask._GetImpl()->_GetExceptionHolder();
                    try {
                        holder->_RethrowUserException();
                    } catch (std::exception& e) {
                        cerr << "RequestError, caught exception: " << e.what() << endl;
                        return njson{}; // Return empty object
                    }
                }

                return previousTask.get(); // Return object from last task
            });
}
