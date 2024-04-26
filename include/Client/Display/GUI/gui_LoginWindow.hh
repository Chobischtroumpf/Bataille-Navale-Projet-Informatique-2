#pragma once

#include <iostream>
#include <cctype>
#include <QApplication> 
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <qt6/QtWidgets/qpushbutton.h>

#include "login_controller.hh"


class LoginWindow : public QWidget {
    Q_OBJECT //toujours mettre dans une fenêtre

public:
    LoginWindow(std::shared_ptr<GameClient> gameClient);

signals:
    void loginSuccessful();  // Signal qu'on émet pour driver qd on se co pour switch fenetre
    void registrationSuccessful();  
    // ...
private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    LoginController login_controller;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *registerButton; 
    bool isPasswordValid(std::string password);
    bool contientMajuscule(const std::string &str);
};