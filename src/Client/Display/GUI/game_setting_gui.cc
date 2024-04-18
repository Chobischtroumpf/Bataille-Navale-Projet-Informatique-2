#include "game_setting_gui.hh"

GameSetting::GameSetting(std::shared_ptr<GameClient> gameClient): gameClient(gameClient) {
    setWindowTitle("Game Setting");
    resize(800, 600);

    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);

    QLabel *gameNameLabel = new QLabel("Game Name", this);
    mainLayout->addWidget(gameNameLabel, 0, 0, 1, 1);
    QLabel *gameModeLabel = new QLabel("Game Mode", this);
    mainLayout->addWidget(gameModeLabel, 1, 0, 1, 1);
    QLabel *timePerTurnLabel = new QLabel("Time Per Turn", this);
    mainLayout->addWidget(timePerTurnLabel, 2, 0, 1, 1);
    QLabel *timePerGameLabel = new QLabel("Time Per Game", this);
    mainLayout->addWidget(timePerGameLabel, 3, 0, 1, 1);
    QLabel *timeGameLabel = new QLabel("Time Game", this);
    mainLayout->addWidget(timeGameLabel, 4, 0, 1, 1);
    QLabel *spectatorAllowedLabel = new QLabel("Spectator Allowed", this);
    mainLayout->addWidget(spectatorAllowedLabel, 5, 0, 1, 1);

    gameName = new QLineEdit(this);
    mainLayout->addWidget(gameName, 0, 1, 1, 1);

    gameMode = new QComboBox(this);
    QStringList mode;
    mode << "Classic" << "Commander";
    gameMode->addItems(mode);
    mainLayout->addWidget(gameMode, 1, 1, 1, 1);

    timePerTurn = new QSlider(Qt::Horizontal, this);
    timePerTurn->setMinimum(5);
    timePerTurn->setMaximum(100);
    timePerTurn->setSingleStep(5);
    timePerTurn->setValue(30);
    connect(timePerTurn, &QSlider::valueChanged, this, &GameSetting::changeValueTimePerTurn);
    mainLayout->addWidget(timePerTurn, 2, 1, 2, 1);

    timePerGame = new QSlider(Qt::Horizontal, this);
    timePerGame->setMinimum(30);
    timePerGame->setMaximum(1000);
    timePerGame->setSingleStep(10);
    timePerGame->setValue(300);
    connect(timePerGame, &QSlider::valueChanged, this, &GameSetting::changeValueTimePerGame);
    mainLayout->addWidget(timePerGame, 3, 1, 2, 1);

    timeGame = new QSlider(Qt::Horizontal, this);
    timeGame->setMinimum(60);
    timeGame->setMaximum(2000);
    timeGame->setSingleStep(10);
    timeGame->setValue(600);
    connect(timeGame, &QSlider::valueChanged, this, &GameSetting::changeValueTimeGame);
    mainLayout->addWidget(timeGame, 4, 1, 2, 1);

    spectatorAllowed = new QComboBox(this);
    QStringList options;
    options << "Yes" << "No";
    spectatorAllowed->addItems(options);
    mainLayout->addWidget(spectatorAllowed, 5, 1, 1, 1);

    timePerTurnValue = new QLabel("30", this);
    mainLayout->addWidget(timePerTurnValue, 2, 3, 1, 1);
    timePerGameValue = new QLabel("300", this);
    mainLayout->addWidget(timePerGameValue, 3, 3, 1, 1);
    timeGameValue = new QLabel("600", this);
    mainLayout->addWidget(timeGameValue, 4, 3, 1, 1);

    backToMenu = new QPushButton("Back", this);
    connect(backToMenu, &QPushButton::clicked, this, &GameSetting::onBackToMenuButtonClicked);
    mainLayout->addWidget(backToMenu, 6, 0, 1, 1);

    toLobby = new QPushButton("Next", this);
    connect(toLobby, &QPushButton::clicked, this, &GameSetting::onGoToLobbyButtonClicked);
    mainLayout->addWidget(toLobby, 6, 3, 1, 1);
}

void GameSetting::onBackToMenuButtonClicked() {
    emit goBackToMenu();
    this->close();
}

void GameSetting::onGoToLobbyButtonClicked() {
    //emit goToLobby();
    this->close();
}

void GameSetting::changeValueTimePerTurn() {
    int value = timePerTurn->value();
    timePerTurnValue->setText(QString::number(value));
}

void GameSetting::changeValueTimePerGame() {
    int value = timePerGame->value();
    timePerGameValue->setText(QString::number(value));
}

void GameSetting::changeValueTimeGame() {
    int value = timeGame->value();
    timeGameValue->setText(QString::number(value));
}