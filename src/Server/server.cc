#include "game_server.hh"

int main() {
  std::string address = "http://localhost:8080"; // Change the address/port as needed
#ifdef OUTPUT_DEBUG
  std::cout << "Debug mode enabled" << std::endl;
  std::ofstream ofs("/tmp/battleship_server.log", std::ofstream::out | std::ofstream::app); // Create a log file and open it as output stream in append mode
#else
  std::cout << "Debug mode disabled" << std::endl;
  std::ofstream ofs("/dev/null", std::ofstream::out); // sends log to /dev/null
#endif

  auto clog_buf = std::clog.rdbuf(); // Save the original std::clog buffer
  std::clog.rdbuf(ofs.rdbuf()); // Redirect std::clog to the log file

  std::cout << "Initializing GameServer" <<std::endl;
  std::clog << "Initializing GameServer" << std::endl;

  GameServer server(address);

  std::cout << "Starting server" <<std::endl;
  try {
    server.initialize(); // Start the server
  } catch (const std::exception& e) {
    std::cerr << "Error starting server: " << e.what() << std::endl;
    std::clog << "Error starting server: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "Press Enter to exit." << std::endl;
  std::string line;
  std::getline(std::cin, line);

  std::cout << "Stopping server" <<std::endl;

  std::clog.rdbuf(clog_buf); // Restore the original std::clog buffer
  ofs.close(); // Close the log file

  return 0;
}
