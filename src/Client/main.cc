#include <iostream>
#include <memory>

#include "driver.hh"

int main(int argc, char *argv[]) {
  std::string server_address;

  if (argc < 2) {
    server_address = "http://localhost:8080";
  } else {
    server_address = argv[1];
  }
  std::cout << "Server address: " << server_address << std::endl;
#ifdef OUTPUT_DEBUG
  std::cout << "Debug mode enabled" << std::endl;
  std::ofstream ofs("/tmp/battleship_client.log", std::ofstream::out | std::ofstream::app); // Create a log file and open it as output stream in append mode
#else
  std::cout << "Debug mode disabled" << std::endl;
  std::ofstream ofs("/dev/null", std::ofstream::out | std::ofstream::app); //sends log to /dev/null
#endif
  auto clog_buf = std::clog.rdbuf(); // Save the original std::clog buffer
  std::clog.rdbuf(ofs.rdbuf()); // Redirect std::clog to the log file
  Driver app(CONSOLE, server_address);
  // Lancement de l'application
  app.launchApp();

  std::clog.rdbuf(clog_buf); // Restore the original std::clog buffer
  ofs.close(); // Close the log file

  return 0;
}