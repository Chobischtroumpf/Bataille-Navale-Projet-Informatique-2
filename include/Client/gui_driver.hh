#pragma once

#include <memory>
#include <QObject>
#include "gui_LoginWindow.hh"
#include "gui_MainMenu.hh"
#include "chat_out_game_gui.hh"
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
    std::unique_ptr<ChatOut> _chatOutWindow;

    const std::string _destination{""}; // Needs to be adapted, dummy value for now

	void showLoginWindow();
	void showMainMenu();
    void showChatOutWindow();

};
