#include "game_server.hh"

int main() {
    std::string address = "http://localhost:8080"; // Change the address/port as needed

    std::cout << "Initializing GameServer" <<std::endl;
    GameServer server(address);

    std::cout << "Starting server" <<std::endl;
    server.initialize(); // Start the server

    std::cout << "Press Enter to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);

    return 0;
}
