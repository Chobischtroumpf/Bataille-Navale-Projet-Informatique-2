#include "game_setting_gui.hh"

GameSetting::GameSetting(std::shared_ptr<GameClient> gameClient): gameClient(gameClient) {
    setWindowTitle("Game Setting");
    resize(800, 600);

    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);

    QFont font("Arial", 14);

    QLabel *gameNameLabel = new QLabel("Game Name", this);
    gameNameLabel->setFont(font);
    mainLayout->addWidget(gameNameLabel, 0, 0, 1, 1);
    QLabel *gameModeLabel = new QLabel("Game Mode", this);
    gameModeLabel->setFont(font);
    mainLayout->addWidget(gameModeLabel, 1, 0, 1, 1);
    QLabel *timePerTurnLabel = new QLabel("Time Per Turn", this);
    timePerTurnLabel->setFont(font);
    mainLayout->addWidget(timePerTurnLabel, 2, 0, 1, 1);
    QLabel *timePerGameLabel = new QLabel("Time Per Game", this);
    timePerGameLabel->setFont(font);
    mainLayout->addWidget(timePerGameLabel, 3, 0, 1, 1);
    QLabel *timeGameLabel = new QLabel("Time Game", this);
    timeGameLabel->setFont(font);
    mainLayout->addWidget(timeGameLabel, 4, 0, 1, 1);
    QLabel *spectatorAllowedLabel = new QLabel("Spectator Allowed", this);
    spectatorAllowedLabel->setFont(font);
    mainLayout->addWidget(spectatorAllowedLabel, 5, 0, 1, 1);

    gameName = new QLineEdit(this);
    gameName->setFixedSize(380, 40);
    gameName->setFont(font);
    gameName->setPlaceholderText(QString("Type here"));
    mainLayout->addWidget(gameName, 0, 1, 1, 2, Qt::AlignHCenter);

    classicMode = new QPushButton("Classic", this);
    classicMode->setFont(font);
    classicMode->setFixedSize(180, 50);
    connect(classicMode, &QPushButton::clicked, this, &GameSetting::classicModeButtonClicked);
    classicMode->setEnabled(false);
    mainLayout->addWidget(classicMode, 1, 1, 1, 1, Qt::AlignHCenter);

    commanderMode = new QPushButton("Commander", this);
    commanderMode->setFont(font);
    commanderMode->setFixedSize(180, 50);
    connect(commanderMode, &QPushButton::clicked, this, &GameSetting::commanderModeButtonClicked);
    mainLayout->addWidget(commanderMode, 1, 2, 1, 1, Qt::AlignHCenter);

    timePerTurn = new QSlider(Qt::Horizontal, this);
    timePerTurn->setFixedSize(400, 30);
    timePerTurn->setMinimum(1);
    timePerTurn->setMaximum(20);
    timePerTurn->setValue(6);
    connect(timePerTurn, &QSlider::valueChanged, this, &GameSetting::changeValueTimePerTurn);
    mainLayout->addWidget(timePerTurn, 2, 1, 1, 2, Qt::AlignHCenter);

    timePerGame = new QSlider(Qt::Horizontal, this);
    timePerGame->setFixedSize(400, 30);
    timePerGame->setMinimum(3);
    timePerGame->setMaximum(100);
    timePerGame->setValue(30);
    connect(timePerGame, &QSlider::valueChanged, this, &GameSetting::changeValueTimePerGame);
    mainLayout->addWidget(timePerGame, 3, 1, 1, 2, Qt::AlignHCenter);

    timeGame = new QSlider(Qt::Horizontal, this);
    timeGame->setFixedSize(400, 30);
    timeGame->setMinimum(6);
    timeGame->setMaximum(200);
    timeGame->setValue(60);
    connect(timeGame, &QSlider::valueChanged, this, &GameSetting::changeValueTimeGame);
    mainLayout->addWidget(timeGame, 4, 1, 1, 2, Qt::AlignHCenter);

    allowSpectator = new QPushButton("Yes", this);
    allowSpectator->setFont(font);
    allowSpectator->setFixedSize(180, 50);
    connect(allowSpectator, &QPushButton::clicked, this, &GameSetting::allowSpectatorButtonClicked);
    allowSpectator->setEnabled(false);
    mainLayout->addWidget(allowSpectator, 5, 1, 1, 1, Qt::AlignHCenter);

    notAllowSpectator = new QPushButton("No", this);
    notAllowSpectator->setFont(font);
    notAllowSpectator->setFixedSize(180, 50);
    connect(notAllowSpectator, &QPushButton::clicked, this, &GameSetting::notAllowSpectatorButtonClicked);
    mainLayout->addWidget(notAllowSpectator, 5, 2, 1, 1, Qt::AlignHCenter);

    timePerTurnValue = new QLabel("30 second      ", this);
    timePerTurnValue->setFont(font);
    mainLayout->addWidget(timePerTurnValue, 2, 3, 1, 1, Qt::AlignRight);
    timePerGameValue = new QLabel("300 second      ", this);
    timePerGameValue->setFont(font);
    mainLayout->addWidget(timePerGameValue, 3, 3, 1, 1, Qt::AlignRight);
    timeGameValue = new QLabel("600 second      ", this);
    timeGameValue->setFont(font);
    mainLayout->addWidget(timeGameValue, 4, 3, 1, 1, Qt::AlignRight);
    noGameNameWarning = new QLabel("", this);
    noGameNameWarning->setFont(font);
    noGameNameWarning->setStyleSheet("color: red;");
    mainLayout->addWidget(noGameNameWarning, 0, 3, 1, 1, Qt::AlignRight);

    backToMenu = new QPushButton("Back", this);
    backToMenu->setFont(font);
    backToMenu->setFixedSize(180, 50);
    connect(backToMenu, &QPushButton::clicked, this, &GameSetting::onBackToMenuButtonClicked);
    mainLayout->addWidget(backToMenu, 6, 0, 1, 1);

    toLobby = new QPushButton("Next", this);
    toLobby->setFont(font);
    toLobby->setFixedSize(180, 50);
    connect(toLobby, &QPushButton::clicked, this, &GameSetting::onGoToLobbyButtonClicked);
    mainLayout->addWidget(toLobby, 6, 3, 1, 1, Qt::AlignRight);
}

void GameSetting::onBackToMenuButtonClicked() {
    emit goBackToMenu();
    this->close();
}

void GameSetting::onGoToLobbyButtonClicked() {
    gameNameString = gameName->text().toStdString();

    if (gameNameString == "") {
        noGameNameWarning->setText("No Game Name !");
    } else {
        njson gameDetails = {{"name", gameNameString},
                             {"gamemode", gameMode},
                             {"gameTimeLimit", timePerTurn->value() * 5},
                             {"playerTimeLimit", timePerGame->value() * 10},
                             {"turnTimeLimit", timeGame->value() * 10},
                             {"maxPlayers", (spectatorAllowed ? 8 : 2)}
                             };
        auto resultFuture = gameClient->CreateGame(gameDetails);
        auto gameID = resultFuture.get();
        if (gameMode == "Commander") {
          nlohmann::json req;
          req["moveType"] = "chooseFaction";
          req["faction"] = 0;
          gameClient->MakeMove(gameID, req);
        }
        emit goToLobby(gameID, true);
        this->close();
    }
}

void GameSetting::classicModeButtonClicked() {
    classicMode->setEnabled(false);
    commanderMode->setEnabled(true);
    gameMode = "Classic";
}

void GameSetting::commanderModeButtonClicked() {
    commanderMode->setEnabled(false);
    classicMode->setEnabled(true);
    gameMode = "Commander";
}

void GameSetting::changeValueTimePerTurn() {
    int value = timePerTurn->value() * 5;
    timePerTurnValue->setText(QString::number(value) + " second      ");
}

void GameSetting::changeValueTimePerGame() {
    int value = timePerGame->value() * 10;
    timePerGameValue->setText(QString::number(value) + " second      ");
}

void GameSetting::changeValueTimeGame() {
    int value = timeGame->value() * 10;
    timeGameValue->setText(QString::number(value) + " second      ");
}

void GameSetting::allowSpectatorButtonClicked() {
    allowSpectator->setEnabled(false);
    notAllowSpectator->setEnabled(true);
    spectatorAllowed = true;
}

void GameSetting::notAllowSpectatorButtonClicked() {
    notAllowSpectator->setEnabled(false);
    allowSpectator->setEnabled(true);
    spectatorAllowed = false;
}

bool GameSetting::isSpectatorAllowed() {
    return spectatorAllowed;
}

std::string GameSetting::getGameName() {
    return gameNameString;
}

bool GameSetting::isCommanderMode() {
    return gameMode == "Commander";
}