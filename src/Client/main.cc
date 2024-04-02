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
  Driver app(CONSOLE, server_address);
  // Lancement de l'application
  app.launchApp();

  return 0;
}