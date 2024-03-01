#include "GameServer.hpp"


using namespace std;

int main() {
    std::string address = "http://localhost:8080"; // Change the address/port as needed

    cout << "Initializing GameServer" << endl;
    GameServer server(address);

    cout << "Starting server" << endl;
    server.initialize(); // Start the server

    std::cout << "Press Enter to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);

    return 0;
}
