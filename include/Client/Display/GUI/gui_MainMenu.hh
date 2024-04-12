#pragma once

#include <iostream>
#include <QApplication> 
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <qt6/QtWidgets/qpushbutton.h>

#include "login_controller.hh"


class MainMenu : public QWidget {
    Q_OBJECT //toujours mettre dans une fenêtre

public:
    MainMenu(std::shared_ptr<GameClient> gameClient);

private slots: // en ref à Qt6
    void onCreatGameButtonClicked();
    void onAddFriendButtonClicked();
	void onChatWithAFriendButtonClicked();
	void onJoinGameButtonClicked();
	void onLogOutButtonClicked();

private:
    LoginController main_menu_controller;
    QPushButton *creatGame;
    QPushButton *addFriend;
	QPushButton *chatWithAFriend;
	QPushButton *joinGame;
	QPushButton *logOut;
};