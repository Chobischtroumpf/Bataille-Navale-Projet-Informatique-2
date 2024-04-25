#pragma once

#include <memory>
#include <QObject>
#include "gui_LoginWindow.hh"
#include "gui_MainMenu.hh"
#include "gui_Game.hh"
#include "chat_out_game_gui.hh"
#include "game_setting_gui.hh"
#include "lobby_gui.hh"
#include "game_client.hh"
#include "gui_review.hh"
#include "gui_review_game.hh"

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
    std::unique_ptr<Game> _gameWindow;
    std::shared_ptr<GameSetting> _gameSettingWindow;
    std::unique_ptr<Lobby> _lobbyWindow;
    std::unique_ptr<Review> _review_menu;
    std::unique_ptr<ReviewGame> _review_game;

    const std::string _destination{""}; // Needs to be adapted, dummy value for now

	void showLoginWindow();
	void showMainMenu();
    void showChatOutWindow(const std::string &destination);
    void showGameWindow(std::string gameId);
    void showGameSettingWindow();
    void showLobbyWindow(std::string gameId, bool admin);
    void showReviewMenu();
    void showReviewGame(std::string gameId);
};
