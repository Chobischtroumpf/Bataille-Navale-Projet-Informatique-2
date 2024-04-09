#include <iostream>
#include <memory>
#include <cstring>

#include "driver.hh"


int main(int argc, char* argv[]) {
    if (std::strcmp(argv[1], "console") == 0) {
        Driver app(CONSOLE);
        // Lancement de l'application
        app.launchApp();
    }
    else if (std::strcmp(argv[1], "gui") == 0) {
        QApplication qtApp(argc, argv);

        QMainWindow mainWindow;
        mainWindow.resize(800, 600);
        mainWindow.setWindowTitle("Please work");
        mainWindow.show();
        //Driver app(GUI);
        // Lancement de l'application
        //app.launchApp(argc, argv);

        return qtApp.exec();
    }
    return 0;
}