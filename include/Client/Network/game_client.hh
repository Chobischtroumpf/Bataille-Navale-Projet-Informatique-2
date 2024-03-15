#pragma once


#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_msg.h>
#include <nlohmann/json.hpp>

#include <memory> // For unique_ptr
#include <future> // For future
#include <vector>


// Declare the njson namespace alias for nlohmann::json 
using njson = nlohmann::json;

class GameClient {
public:
    explicit GameClient(const std::string& baseUri);
    
    
    std::future<std::string> CreateGame(const njson& gameDetails);
    std::future<njson> JoinGame(const std::string& sessionId);
    std::future<bool> MakeMove(const std::string& sessionId, const std::string& move);
    std::future<bool> Login(const std::string& userId, const std::string& password);
    std::future<bool> Register(const std::string& userId, const std::string& password);
    std::future<std::string> GetUserId(const std::string& username);
    std::future<std::string> GetUsername(const std::string& userId);
    std::future<std::string> GetGames();
    std::future<njson> QueryGameState(const std::string& sessionId);
    std::future<bool> SendMessage(const std::string& targetId, const std::string& message);
    std::future<njson> GetMessages(const std::string& recipientId);
    std::future<bool> AddFriend(const std::string& username);
    std::future<njson> GetFriends();
private:
    // Use unique_ptr for automatic resource management of http_client
    // Makes it possible to "initialize" the http client in the constructor, allowing easier debugging
    std::unique_ptr<web::http::client::http_client> client;
    
    std::string authToken; // Stores the authToken if the user is logged in

    // Test requests to be deleted later
    void TestRequest();
    std::future<void> TestRequest2();
    std::future<void> TestRequest3();

    // Helper method to add Authorization header to requests
    void AddAuthHeader(web::http::http_request& request);

    pplx::task<njson> PostRequest(const std::string& path, const njson& data);
    pplx::task<njson> GetRequest(const std::string& path);
};