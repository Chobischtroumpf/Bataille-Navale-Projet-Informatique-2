#include <iostream>
#include <memory>
#include <cstring>

#include "driver.hh"
#include "gui_driver.hh"
//#include "login_display.hh"
/*
int print(QApplication &qtApp){
        QMainWindow mainWindow;
        mainWindow.resize(800, 600);
        mainWindow.setWindowTitle("Please work");
        mainWindow.show();
        std::cout<<"hello"<< std::endl;
        return qtApp.exec();
}
*/

int main(int argc, char *argv[]) {
#ifdef OUTPUT_DEBUG
  std::cout << "Debug mode enabled" << std::endl;
  std::ofstream ofs("/tmp/battleship_client.log",
                    std::ofstream::out |
                        std::ofstream::app); // Create a log file and open it as
                                             // output stream in append mode
#else
  std::cout << "Debug mode disabled" << std::endl;
  std::ofstream ofs("/dev/null",
                    std::ofstream::out |
                        std::ofstream::app); // sends log to /dev/null
#endif
  auto clog_buf = std::clog.rdbuf(); // Save the original std::clog buffer
  std::clog.rdbuf(ofs.rdbuf());      // Redirect std::clog to the log file

  if (argc == 1) {
    Driver app(CONSOLE, "http://localhost:8080");
    // Lancement de l'application
    app.launchApp();
  } else if (std::strcmp(argv[1], "gui") == 0) {
    QApplication qtApp(argc, argv);

    DriverGui app;
    // Lancement de l'application
    app.run();

    qtApp.exec();
  } else {
    Driver app(CONSOLE, argv[1]);
    app.launchApp();
  }

  std::clog.rdbuf(clog_buf); // Restore the original std::clog buffer
  ofs.close(); // Close the log file

  return 0;
}
