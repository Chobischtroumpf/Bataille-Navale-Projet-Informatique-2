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
        {"gamemode", "Classic"},
        {"gameTimeLimit", 30},
        {"turnTimeLimit", 2},
        {"playerTimeLimit", 2},
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
    auto sessionId = createGameFuture.get();
    cout << "Game created with session ID: " << sessionId << "\n";

    // Query Game State
    waitForEnter("Ready to query game state. ");
    auto queryGameStateFuture = gameClient.QueryGameState(sessionId);
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


    waitForEnter("Ready to send message.");
    // Asynchronously 
    auto messageSentFuture = gameClient.SendMessage("1","Hello");
    // Correctly wait for the future to get the result and print it
    auto result = messageSentFuture.get(); // This blocks until the future is ready and gets the result
    cout << "Message sent: " << result << "\n";
    
    waitForEnter("Ready to get messages.");
    // Asynchronously 
    auto messagesFuture = gameClient.GetMessages("1");
    // Correctly wait for the future to get the result and print it
    auto result2 = messagesFuture.get(); // This blocks until the future is ready and gets the result
    cout << "Messages retrieved: " << result2 << "\n";
    cout << "All requests were processed. Check your server for the responses.\n";


   

    njson endGameMove = {
        {"moveType", "EndGame"},
    };
    waitForEnter("Ready to test the JoinGame. ");
    GameClient gameClient2("http://localhost:8080");
    auto registerFuture2 = gameClient2.Register("newUser2", "newPassword2");
    if (registerFuture2.get()) {
        cout << "Register request success.\n";
    } else {
        cout << "Register request failed.\n";
    }
    auto loginFuture2 = gameClient2.Login("newUser2", "newPassword2");
    cout << "Login request success: " << loginFuture2.get() << "\n";
    auto joinGamefuture = gameClient2.JoinGame(sessionId);
    cout << "Join game result: " << joinGamefuture.get().dump() << "\n";


     njson startGameMove = {
        {"moveType", "StartGame"},
    };

    // Call the MakeMove method
    waitForEnter("Ready to test the StartGame move. ");
    auto makeMoveFuture = gameClient.MakeMove(sessionId, startGameMove);
    cout << "Move result: " << makeMoveFuture.get() << "\n";
    
    // Call the MakeMove method
    waitForEnter("Ready to test the EndGame move. ");
    auto endMoveFuture = gameClient.MakeMove(sessionId, endGameMove);
    cout << "Move result: " << endMoveFuture.get() << "\n";

    // Get Games Information
    waitForEnter("Ready to get game list. ");
    auto getGamesFuture = gameClient.GetGames();
    cout << "Games information received: " << getGamesFuture.get() << "\n";

    // Get the game history
    waitForEnter("Ready to test the game history retrieval ");
    auto historyFuture = gameClient.GetGameHistory(sessionId);
    cout << "game history: " << historyFuture.get() << "\n";
    return 0;
}
