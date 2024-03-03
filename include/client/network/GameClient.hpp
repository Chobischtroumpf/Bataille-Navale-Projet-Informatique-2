#ifndef GAMECLIENT_HPP
#define GAMECLIENT_HPP

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory> // For unique_ptr
#include <future>           // For future


// Declare the njson namespace alias for nlohmann::json 
using njson = nlohmann::json;

class GameClient {
public:
    explicit GameClient(const std::string& baseUri);
    
    
    std::future<std::string> CreateGame(const njson& gameDetails);
    std::future<njson> JoinGame(const std::string& sessionId);
    std::future<bool> MakeMove(const std::string& sessionId, const std::string& move);
    std::future<std::string> Login(const std::string& userId, const std::string& password);
    std::future<std::string> GetUserId(const std::string& username);
    std::future<std::string> GetGames();
    std::future<njson> QueryGameState(const std::string& sessionId, const std::string& userId);
    std::future<bool> SendMessage(const std::string& senderId, const std::string& targetId, const std::string& message);
    std::future<nlohmann::json> GameClient::GetMessages(const std::string& recipientId);
private:
    // Use unique_ptr for automatic resource management of http_client
    // Makes it possible to "initialize" the http client in the constructor, allowing easier debugging
    std::unique_ptr<web::http::client::http_client> client;
    
    // Test requests to be deleted later
    void TestRequest();
    std::future<void> TestRequest2();
    std::future<void> TestRequest3();

    pplx::task<njson> PostRequest(const std::string& path, const njson& data);
    pplx::task<njson> GetRequest(const std::string& path);
};

#endif // GAMECLIENT_HPP
