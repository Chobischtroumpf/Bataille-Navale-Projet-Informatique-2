#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <qt6/QtWidgets/qpushbutton.h>

#include "game_client.hh"

class GameSetting : public QWidget {
    Q_OBJECT

public:
    GameSetting(std::shared_ptr<GameClient> gameClient);

signals:
    // insérer les signaux ici
    void goBackToMenu();
    //void goToLobby();

private slots: // en ref à Qt6
    void onBackToMenuButtonClicked();
    void onGoToLobbyButtonClicked();
    void changeValueTimePerTurn();
    void changeValueTimePerGame();
    void changeValueTimeGame();

private:
    std::shared_ptr<GameClient> gameClient;
    QLineEdit *gameName;
    QComboBox *gameMode;
    QSlider *timePerTurn;
    QSlider *timePerGame;
    QSlider *timeGame;
    QLabel *timePerTurnValue;
    QLabel *timePerGameValue;
    QLabel *timeGameValue;
    QComboBox *spectatorAllowed;

    QPushButton *backToMenu;
    QPushButton *toLobby;
};
