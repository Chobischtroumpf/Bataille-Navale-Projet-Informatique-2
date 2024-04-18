#include "driver.hh"

int main(int argc, char *argv[]) {

#ifdef OUTPUT_DEBUG
  std::ofstream out("battleship.log");
  auto coutbuf = std::cerr.rdbuf(out.rdbuf()); //save and redirect
#endif
  std::string server_address;
  if (argc < 2) {
    server_address = "http://localhost:8080";
  } else {
    server_address = argv[1];
  }
  Driver app(CONSOLE, server_address);
  // Lancement de l'application
  app.launchApp();

#ifdef OUTPUT_DEBUG
  std::cerr.rdbuf(coutbuf); //reset to standard output again
#endif
  return 0;
}