#include "game_client.hh" // Header file for GameClient
#include <iostream>
#include <nlohmann/json.hpp>
#include <future> // For std::async and std::future

using njson = nlohmann::json;
using namespace std;

void waitForEnter(const string& prompt = "Press Enter to continue...") {
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    cout << "Program started\n";

    // Initialize the GameClient with the base URI of your game server
    GameClient gameClient("http://localhost:8080");

    cout << "Instance successfully started\n";

    // Create a fictional njson object with game details
    njson gameDetails = {
        {"name", "New Room"},
        {"type", "Classic"},
        {"gameTimeLimit", 30},
        {"turnTimeLimit", 2},
        {"maxPlayers", 8}
    };

    // Registration
    waitForEnter("Ready to register. ");
    auto registerFuture = gameClient.Register("newUser", "newPassword");
    if (registerFuture.get()) {
        cout << "Register request success.\n";
    } else {
        cout << "Register request failed.\n";
    }

    // Login
    waitForEnter("Ready to login. ");
    auto loginFuture = gameClient.Login("newUser", "newPassword");
    cout << "Login request success: " << loginFuture.get() << "\n";

    // Create Game
    waitForEnter("Ready to create game. ");
    auto createGameFuture = gameClient.CreateGame(gameDetails);
    cout << "Game created with session ID: " << createGameFuture.get() << "\n";

    // Get Games Information
    waitForEnter("Ready to get game list. ");
    auto getGamesFuture = gameClient.GetGames();
    cout << "Games information received: " << getGamesFuture.get() << "\n";

    // Query Game State
    waitForEnter("Ready to query game state. ");
    auto queryGameStateFuture = gameClient.QueryGameState("exampleSessionId");
    cout << "Game state for session 'exampleSessionId': " << queryGameStateFuture.get().dump() << "\n";

    // Add a Friend
    waitForEnter("Ready to add friend. ");
    auto addFriendFuture = gameClient.AddFriend("friendUsername");
    cout << "Friend was added ? : " << addFriendFuture.get() << "\n";
   

    // Get Friends List
    waitForEnter("Ready to get friend list. ");
    auto getFriendsFuture = gameClient.GetFriends();
    cout << "Friend list received: " << getFriendsFuture.get().dump() << "\n";

    waitForEnter("Ready to get user id. ");
    // Asynchronously get user ID for the username
    auto getUserIdFuture = gameClient.GetUserId("newUser");
    // Correctly wait for the future to get the result and print it
    auto userId = getUserIdFuture.get(); // This blocks until the future is ready and gets the result
    cout << "UserId received: " << userId << "\n";
    
    cout << "All requests were processed. Check your server for the responses.\n";

    return 0;
}
