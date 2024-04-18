#include "game_server.hh"

using namespace std;

int main() {
#ifdef OUTPUT_DEBUG
  std::ofstream out("server.log");
  auto coutbuf = std::cerr.rdbuf(out.rdbuf()); //save and redirect
#endif
  std::string address = "http://localhost:8080"; // Change the address/port as needed

  cout << "Initializing GameServer" << endl;
  GameServer server(address);

  cout << "Starting server" << endl;
  server.initialize(); // Start the server

  std::cout << "Press Enter to exit." << std::endl;
  std::string line;
  std::getline(std::cin, line);

#ifdef OUTPUT_DEBUG
  std::cerr.rdbuf(coutbuf); //reset to standard output again
#endif

  return 0;
}
