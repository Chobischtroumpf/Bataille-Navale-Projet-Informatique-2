#pragma once

#include <iostream>
#include <QApplication> 
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class LoginWindow : public QWidget {
    Q_OBJECT //toujours mettre dans une fenêtre

public:
    LoginWindow();

private slots: // en ref à Qt6
    void onLoginButtonClicked();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
};