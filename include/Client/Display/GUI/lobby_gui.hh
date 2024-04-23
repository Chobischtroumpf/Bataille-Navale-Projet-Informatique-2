#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <tuple>

#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QFont>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer>
#include <QPushButton>
#include <qt6/QtWidgets/qpushbutton.h>
#include <QRegularExpression>

#include "game_setting_gui.hh"
#include "lobby_view.hh"
#include "lobby_controller.hh"

class Lobby : public QWidget {
    Q_OBJECT

public:
    explicit Lobby(const std::string &session_id, 
                   std::shared_ptr<GameClient> client,
                   bool admin,
                   std::shared_ptr<GameSetting> gameSettingConsole);
    ~Lobby();
    void loadParameters(std::shared_ptr<GameSetting> gameSetting);
    void loadParameters(const std::string &session_id);

signals:
    // insérer les signaux ici
    void goBackToMenu();
    void launchGame(std::string gameId);

private slots: // en ref à Qt6
    void onBackToMenuButtonClicked();
    void onLaunchGameButtonClicked();

    void onFactionBombardementClicked();
    void onFactionSonarClicked();
    void onFactionMinesClicked();

    void updatePlayer();
    void clearPlayerLayout();
    void invitePlayer();
    void checkGameStart();

private:
    std::string _session_id;
    std::shared_ptr<LobbyView> _view;
    std::shared_ptr<LobbyController> _controller;
    bool _admin;
    int _selected_faction = 0;
    bool _faction_chosen = false;
    int _max_players;
    std::string _game_name;
    bool _commander_mode;

    QTimer *timerPlayer;
    QTimer *timerGameStart;
    QLabel *warning;
    QVBoxLayout *scrollLayoutPlayer;
    QLineEdit *addPlayerName;
    QPushButton *FactionBombardement;
    QPushButton *FactionSonar;
    QPushButton *FactionMines;

    QPushButton *backToMenu;
    QPushButton *toGame;
};
