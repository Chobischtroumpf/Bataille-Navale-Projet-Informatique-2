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


class LoginWindow : public QWidget {
    Q_OBJECT //toujours mettre dans une fenêtre

public:
    LoginWindow(std::shared_ptr<GameClient> gameClient);

private slots: // en ref à Qt6
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    LoginController login_controller;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;  
};