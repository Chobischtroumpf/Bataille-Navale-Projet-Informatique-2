#include "lobby_gui.hh"

Lobby::Lobby(const std::string &session_id,
             std::shared_ptr<GameClient> client,
             bool admin, 
             std::shared_ptr<GameSetting> gameSetting = nullptr)
             : _session_id(session_id) , _admin(admin) {
    _view = std::make_shared<LobbyView>(client);
    _controller = std::make_shared<LobbyController>(client);
    if (gameSetting != nullptr) {
        loadParameters(gameSetting);
    } else {
        loadParameters(session_id);
    }

    setWindowTitle(QString::fromStdString("Lobby (" + _game_name + ")"));
    resize(800, 600);

    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);

    QFont font("Arial", 14);

    timerPlayer = new QTimer(this);
    connect(timerPlayer, &QTimer::timeout, this, &Lobby::updatePlayer);
    timerPlayer->setInterval(1000); // 1 secondes
    timerPlayer->start();
    
    QScrollArea *scrollAreaPlayer = new QScrollArea(this);
    QWidget *scrollWidgetPlayer = new QWidget(scrollAreaPlayer);
    scrollLayoutPlayer = new QVBoxLayout(scrollWidgetPlayer);
    scrollWidgetPlayer->setLayout(scrollLayoutPlayer);
    scrollAreaPlayer->setWidgetResizable(true);
    scrollAreaPlayer->setWidget(scrollWidgetPlayer);
    scrollAreaPlayer->setFixedWidth(780);
    //scrollAreaPlayer->setFixedHeight(300);
    mainLayout->addWidget(scrollAreaPlayer, 0, 0, 4, 3, Qt::AlignHCenter);
    
    addPlayerName = new QLineEdit(this);
    addPlayerName->setFixedSize(400, 40);
    addPlayerName->setFont(font);
    addPlayerName->setPlaceholderText(QString("Username of the player to add"));
    connect(addPlayerName, &QLineEdit::returnPressed, this, &Lobby::invitePlayer);
    mainLayout->addWidget(addPlayerName, 4, 0, 1, 3, Qt::AlignHCenter);
    
    if (_commander_mode) {
        FactionBombardement = new QPushButton("Faction Bombardement", this);
        FactionBombardement->setFont(font);
        FactionBombardement->setFixedSize(260, 50);
        connect(FactionBombardement, &QPushButton::clicked, this, &Lobby::onFactionBombardementClicked);
        mainLayout->addWidget(FactionBombardement, 5, 0, 1, 1);

        FactionSonar = new QPushButton("Faction Sonar", this);
        FactionSonar->setFont(font);
        FactionSonar->setFixedSize(260, 50);
        connect(FactionSonar, &QPushButton::clicked, this, &Lobby::onFactionSonarClicked);
        mainLayout->addWidget(FactionSonar, 5, 1, 1, 1, Qt::AlignHCenter);

        FactionMines = new QPushButton("Faction Mines", this);
        FactionMines->setFont(font);
        FactionMines->setFixedSize(260, 50);
        connect(FactionMines, &QPushButton::clicked, this, &Lobby::onFactionMinesClicked);
        mainLayout->addWidget(FactionMines, 5, 2, 1, 1, Qt::AlignRight);
    } else if (!_admin) {
        timerGameStart = new QTimer(this);
        connect(timerGameStart, &QTimer::timeout, this, &Lobby::checkGameStart);
        timerGameStart->setInterval(1000); // 1 secondes
        timerGameStart->start();
    }

    Warning = new QLabel("", this);
    Warning->setFont(font);
    Warning->setStyleSheet("color: red;");
    mainLayout->addWidget(Warning, 6, 1, 1, 1, Qt::AlignHCenter);

    backToMenu = new QPushButton("Back", this);
    backToMenu->setFont(font);
    backToMenu->setFixedSize(180, 50);
    connect(backToMenu, &QPushButton::clicked, this, &Lobby::onBackToMenuButtonClicked);
    mainLayout->addWidget(backToMenu, 6, 0, 1, 1);

    toGame = new QPushButton("Lauch Game", this);
    toGame->setFont(font);
    toGame->setFixedSize(180, 50);
    connect(toGame, &QPushButton::clicked, this, &Lobby::onLaunchGameButtonClicked);
    mainLayout->addWidget(toGame, 6, 2, 1, 1, Qt::AlignRight);
}

Lobby::~Lobby() {
    delete timerPlayer;
    if (!_admin) {
        if ((!_commander_mode) or _faction_chosen) {
            delete timerGameStart;
        }
    }
}

void Lobby::onFactionBombardementClicked() {
    int _selected_faction = 0;
    FactionBombardement->setEnabled(false);

    if (!_faction_chosen) {
        _faction_chosen = true;
        if (!_admin) {
            timerGameStart = new QTimer(this);
            connect(timerGameStart, &QTimer::timeout, this, &Lobby::checkGameStart);
            timerGameStart->setInterval(1000); // 1 secondes
            timerGameStart->start();
        }
    }
    else {
        FactionSonar->setEnabled(true);
        FactionMines->setEnabled(true);
    }
}

void Lobby::onFactionSonarClicked() {
    int _selected_faction = 1;
    FactionSonar->setEnabled(false);

    if (!_faction_chosen) {
        _faction_chosen = true;
        if (!_admin) {
            timerGameStart = new QTimer(this);
            connect(timerGameStart, &QTimer::timeout, this, &Lobby::checkGameStart);
            timerGameStart->setInterval(1000); // 1 secondes
            timerGameStart->start();
        }
    }
    else {
        FactionBombardement->setEnabled(true);
        FactionMines->setEnabled(true);
    }
}

void Lobby::onFactionMinesClicked() {
    int _selected_faction = 2;
    FactionMines->setEnabled(false);

    if (!_faction_chosen) {
        _faction_chosen = true;
        if (!_admin) {
            timerGameStart = new QTimer(this);
            connect(timerGameStart, &QTimer::timeout, this, &Lobby::checkGameStart);
            timerGameStart->setInterval(1000); // 1 secondes
            timerGameStart->start();
        }
    }
    else {
        FactionBombardement->setEnabled(true);
        FactionSonar->setEnabled(true);
    }
}

void Lobby::updatePlayer() {
    clearPlayerLayout();
    QFont font("Arial", 14);

    for (auto playerName : _view->getUserInGame(_session_id)) {
        QLabel *playerNameLabel = new QLabel(QString::fromStdString(playerName));
        playerNameLabel->setFont(font);
        playerNameLabel->setStyleSheet("color: blue;");
        scrollLayoutPlayer->addWidget(playerNameLabel);
        scrollLayoutPlayer->addSpacing(20);
    }
}

void Lobby::clearPlayerLayout() {
    QLayoutItem *item;
    while ((item = scrollLayoutPlayer->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
}

void Lobby::invitePlayer() {
    std::string playerName = addPlayerName->text().toStdString();;
    std::string message = "sessionID: " + _session_id;
    _controller->sendIDGame(playerName, message);
    addPlayerName->setText("");
}

void Lobby::checkGameStart() {
    auto messagesJson = _view->getGameState(_session_id);
    if (messagesJson.at("hasStarted") == true) {
        emit launchGame(_session_id);
        this->close();
    }
}

void Lobby::onBackToMenuButtonClicked() {
    emit goBackToMenu();
    this->close();
}

void Lobby::onLaunchGameButtonClicked() {
    if (_view->getUserInGame(_session_id).size() < 2) {
        Warning->setText("Not enough Player !");
    } else if (!_faction_chosen) {
        Warning->setText("No Faction Chosen !");
    } else {
        _controller->launchGame(_session_id);
        emit launchGame(_session_id);
        this->close();
    }
}

void Lobby::loadParameters(std::shared_ptr<GameSetting> gameSetting) {
    _max_players = gameSetting->isSpectatorAllowed() ? 8 : 2;
    _game_name = gameSetting->getGameName();
    _commander_mode = gameSetting->isCommanderMode();
}

void Lobby::loadParameters(const std::string &session_id) {
    nlohmann::json game = _view->getGameState(session_id);

    _commander_mode = game.at("gameState").at("gamemode") == "Commandant";
    _game_name = game.at("sessionName").get<std::string>();
}