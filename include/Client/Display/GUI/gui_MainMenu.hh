#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <tuple>
#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer>
#include <QPushButton>
#include <qt6/QtWidgets/qpushbutton.h>
#include <QRegularExpression>
#include <QEvent> // for QEvent
#include <QMouseEvent> // for QMouseEvent
#include <QEnterEvent> // for QEnterEvent
#include <QFocusEvent> // for QFocusEvent
#include <QPainter>
#include <QMovie>

#include "main_menu_controller.hh"
#include "main_menu_view.hh"
#include "highlight_button.hh"

class HighlightButton : public QPushButton {
    Q_OBJECT
public:
    HighlightButton(QString name, QWidget *parent = nullptr);

signals:
    void mouseEntered();
    void mouseLeft();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};


class MainMenu : public QWidget {
    Q_OBJECT //toujours mettre dans une fenêtre

public:
    MainMenu(std::shared_ptr<GameClient> gameClient);
    ~MainMenu();

signals:
    void userDisconnection();
    void startChat(const std::string& destination);

private slots: // en ref à Qt6
    void onCreatGameButtonClicked();
    void onAddFriendButtonClicked();
	void onChatWithAFriendButtonClicked(const QString &destination);
	void onJoinGameButtonClicked();
	void onLogOutButtonClicked();

    void onFriendLineEditReturnPressed();

    void updateFriends();
    void clearFriendsLayout();
    void updateNotifications();
    void clearNotificationsLayout();
    void updateBackground();

    void mouseOnButton();
    void mouseLeftButton();

private:
    std::shared_ptr<MainMenuController> _controller;
    std::shared_ptr<MainMenuView> _view;
    QTimer *timerFriends;
    QTimer *timerNotifications;
    QTimer *timerBackground;
    QPushButton *creatGame;
    QPushButton *addFriend;
	QPushButton *chatWithAFriend;
	QPushButton *joinGame;
	QPushButton *logOut;

    QVBoxLayout *scrollLayoutFriends; // Layout pour contenir les amis
    QLineEdit *friendNameLineEdit;
    QVBoxLayout *scrollLayoutNotifications;

    int frameCounter;

    // Pour dessiner le background car on hérite pas de QMainWindow
    void paintEvent(QPaintEvent *event) override;
};