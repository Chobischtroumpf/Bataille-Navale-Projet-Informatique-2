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

int main(int argc, char* argv[]) {
    if (std::strcmp(argv[1], "console") == 0) {
        Driver app(CONSOLE);
        // Lancement de l'application
        app.launchApp();
    }
    else if (std::strcmp(argv[1], "gui") == 0) {
        QApplication qtApp(argc, argv);
        
        DriverGui app;
        // Lancement de l'application
        app.run();
        
        return qtApp.exec();
    }
    return 0;
}
