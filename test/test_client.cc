#include "game_client.hh" // Header file for GameClient
#include <iostream>
#include <nlohmann/json.hpp>
#include <future> // For std::async and std::future

using njson = nlohmann::json;
using namespace std;

int main() {
    cout << "Program started" << endl;

    // Initialize the GameClient with the base URI of your game server
    GameClient gameClient("http://localhost:8080");

    cout << "Instance successfully started" << endl;

    // Example usage of asynchronous methods in GameClient
    // Create a fictional njson object with game details
    njson gameDetails = {
        {"name", "New Room"},
        {"type", "Classic"},
        {"gameTimeLimit", 30},
        {"turnTimeLimit", 2},
        {"maxPlayers", 8}
    };
    // auto getValue = gameClient.TestRequest3();
    // getValue.get();
    // cout << "Successfully got the void value" << endl;

    // Tries to register
    auto registerFuture = gameClient.Register("newUser", "newPassword");
    auto registerVal = registerFuture.get();
    if (registerVal) {
        cout << "Register request success." << endl;
    } else {
        cout << "Register request failed." << endl;
    }
    // Tries to login
    auto loginFuture = gameClient.Login("newUser", "newPassword");
    auto loginVal = loginFuture.get();
    cout << "Login request success : " << loginVal << endl;

    // Asynchronously create a game and wait for the session ID
    auto createGameFuture = gameClient.CreateGame(gameDetails);
    auto sessionId = createGameFuture.get();
    cout << "Game created with session ID: " << sessionId << endl;

    // Asynchronously get games information
    auto getGamesFuture = gameClient.GetGames();
    auto gamesInfo = getGamesFuture.get(); // This will wait and retrieve the games information
    cout << "Games information received: " << gamesInfo << endl;

    // Asynchronously query game state
    auto queryGameStateFuture = gameClient.QueryGameState("exampleSessionId");
    auto gameState = queryGameStateFuture.get(); // This will wait and retrieve the game state
    cout << "Game state for session 'exampleSessionId': " << gameState.dump() << endl;

    cout << "All requests were sent. Check your server for the responses." << endl;

    return 0;
}