#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <QApplication>
#include <QWidget>
#include <QFont>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <qt6/QtWidgets/qpushbutton.h>

#include "game_client.hh"

class GameSetting : public QWidget {
    Q_OBJECT

public:
    GameSetting(std::shared_ptr<GameClient> gameClient);
    ~GameSetting();
    bool isSpectatorAllowed();
    std::string getGameName();
    bool isCommanderMode();

signals:
    // insérer les signaux ici
    void goBackToMenu();
    void goToLobby(std::string gameId, bool admin);

private slots: // en ref à Qt6
    void onBackToMenuButtonClicked();
    void onGoToLobbyButtonClicked();
    void classicModeButtonClicked();
    void commanderModeButtonClicked();
    void changeValueTimePerTurn();
    void changeValueTimePerGame();
    void changeValueTimeGame();
    void allowSpectatorButtonClicked();
    void notAllowSpectatorButtonClicked();

private:
    std::shared_ptr<GameClient> gameClient;
    std::string gameNameString;
    std::string gameMode = "Classic";
    bool spectatorAllowed = true;

    QLineEdit *gameName;
    QLabel *noGameNameWarning;
    QPushButton *classicMode;
    QPushButton *commanderMode;
    QSlider *timePerTurn;
    QSlider *timePerGame;
    QSlider *timeGame;
    QLabel *timePerTurnValue;
    QLabel *timePerGameValue;
    QLabel *timeGameValue;
    QPushButton *allowSpectator;
    QPushButton *notAllowSpectator;

    QPushButton *backToMenu;
    QPushButton *toLobby;
};
