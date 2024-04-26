#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer> // Peut servir pour ajouter un refresh automatisé tout les x temps
#include <QTextEdit>
#include <qt6/QtWidgets/qpushbutton.h>
#include <QScrollBar>
#include "chat_controller.hh"
#include "chat_view.hh"
#include "message.hh"

class ChatOut : public QWidget {
    Q_OBJECT  // Nécessaire pour les fonctionnalités MOC de Qt

public:
    explicit ChatOut(const std::string &destination, std::shared_ptr<GameClient> gameClient);

signals:
    void goBackToMenu();
    void messageSent(const QString &msg);
    void errorOccurred(const QString &error);  

private slots:
    void onBackToMenuButtonClicked();

private:
    std::shared_ptr<ChatController> _controller;
    std::shared_ptr<ChatView> _view;
    std::string _destination;

    QPushButton *backToMenu;  
    QTextEdit *messageDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;

    void updateMessagesDisplay();
    void sendMessage();
};