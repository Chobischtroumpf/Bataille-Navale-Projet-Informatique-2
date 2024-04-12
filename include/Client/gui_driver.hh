#pragma once

#include <memory>
#include <QObject>
#include "gui_LoginWindow.hh"
#include "gui_MainMenu.hh"
#include "game_client.hh"

class DriverGui : public QObject {
    Q_OBJECT 
public:
    DriverGui();
    void run();

private:
    std::shared_ptr<GameClient> _game_client;
    std::unique_ptr<LoginWindow> _loginWindow;
	std::unique_ptr<MainMenu> _mainMenuWindow;
	void showLoginWindow();
	void showMainMenu();
};
