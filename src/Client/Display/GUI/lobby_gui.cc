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

    _my_username = client->getClientUsername();

    setWindowTitle(QString::fromStdString("Lobby (" + _game_name + ")"));
    resize(800, 600);

    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);

    QFont font("Arial", 14);
    QFont smallfont("Arial", 8);


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
    mainLayout->addWidget(scrollAreaPlayer, 0, 0, 3, 3, Qt::AlignHCenter);

    QLabel *gameIDLabel = new QLabel(QString::fromStdString("Game ID : " + _session_id), this);
    gameIDLabel->setFont(smallfont);
    gameIDLabel->setFixedSize(520, 40);
    mainLayout->addWidget(gameIDLabel, 4, 0, 1, 2);

    copyToClipboard = new QPushButton("Copy to Clipboard", this);
    copyToClipboard->setFont(font);
    copyToClipboard->setFixedSize(260, 30);
    connect(copyToClipboard, &QPushButton::clicked, this, &Lobby::onCopyToClipboardButtonClicked);
    mainLayout->addWidget(copyToClipboard, 4, 2, 1, 1, Qt::AlignRight);


    if (_commander_mode) {
        factionBombardement = new QPushButton("Faction Bombardement", this);
        factionBombardement->setFont(font);
        factionBombardement->setFixedSize(260, 50);
        connect(factionBombardement, &QPushButton::clicked, this, &Lobby::onFactionBombardementClicked);
        mainLayout->addWidget(factionBombardement, 5, 0, 1, 1);

        factionSonar = new QPushButton("Faction Sonar", this);
        factionSonar->setFont(font);
        factionSonar->setFixedSize(260, 50);
        connect(factionSonar, &QPushButton::clicked, this, &Lobby::onFactionSonarClicked);
        mainLayout->addWidget(factionSonar, 5, 1, 1, 1, Qt::AlignHCenter);

        factionMines = new QPushButton("Faction Mines", this);
        factionMines->setFont(font);
        factionMines->setFixedSize(260, 50);
        connect(factionMines, &QPushButton::clicked, this, &Lobby::onFactionMinesClicked);
        mainLayout->addWidget(factionMines, 5, 2, 1, 1, Qt::AlignRight);
    } else if (!_admin) {
        timerGameStart = new QTimer(this);
        connect(timerGameStart, &QTimer::timeout, this, &Lobby::checkGameStart);
        timerGameStart->setInterval(1000); // 1 secondes
        timerGameStart->start();
    }

    backToMenu = new QPushButton("Back", this);
    backToMenu->setFont(font);
    backToMenu->setFixedSize(180, 50);
    connect(backToMenu, &QPushButton::clicked, this, &Lobby::onBackToMenuButtonClicked);
    mainLayout->addWidget(backToMenu, 6, 0, 1, 1);

    if (_admin) {
        addPlayerName = new QLineEdit(this);
        addPlayerName->setFixedSize(400, 40);
        addPlayerName->setFont(font);
        addPlayerName->setPlaceholderText(QString("Username of the player to add"));
        connect(addPlayerName, &QLineEdit::returnPressed, this, &Lobby::invitePlayer);
        mainLayout->addWidget(addPlayerName, 3, 0, 1, 3, Qt::AlignHCenter);

        warning = new QLabel("", this);
        warning->setFont(font);
        warning->setStyleSheet("color: red;");
        if (_commander_mode) {
            mainLayout->addWidget(warning, 6, 1, 1, 1, Qt::AlignHCenter);
        } else {
            mainLayout->addWidget(warning, 6, 1, 1, 1, Qt::AlignRight);
        }

        toGame = new QPushButton("Lauch Game", this);
        toGame->setFont(font);
        toGame->setFixedSize(180, 50);
        connect(toGame, &QPushButton::clicked, this, &Lobby::onLaunchGameButtonClicked);
        mainLayout->addWidget(toGame, 6, 2, 1, 1, Qt::AlignRight);
    }
}

Lobby::~Lobby() {
    delete timerPlayer;
    clearPlayerLayout();
    delete scrollLayoutPlayer;
    delete copyToClipboard;
    delete backToMenu;

    if (_commander_mode) {
        delete factionBombardement;
        delete factionSonar;
        delete factionMines;
    }

    if (_admin) {
        delete warning;
        delete addPlayerName;
        delete toGame;
    } else if ((!_commander_mode) or _faction_chosen) {
        delete timerGameStart;
    }
}

void Lobby::onCopyToClipboardButtonClicked() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString text_to_be_copied = QString::fromStdString(_session_id);
    clipboard->setText(text_to_be_copied);
}

void Lobby::onFactionBombardementClicked() {
    _selected_faction = 0;
    _controller->sendFaction(_session_id, _selected_faction);
    factionBombardement->setEnabled(false);

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
        factionSonar->setEnabled(true);
        factionMines->setEnabled(true);
    }
}

void Lobby::onFactionSonarClicked() {
    _selected_faction = 1;
    _controller->sendFaction(_session_id, _selected_faction);
    factionSonar->setEnabled(false);

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
        factionBombardement->setEnabled(true);
        factionMines->setEnabled(true);
    }
}

void Lobby::onFactionMinesClicked() {
    _selected_faction = 2;
    _controller->sendFaction(_session_id, _selected_faction);
    factionMines->setEnabled(false);

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
        factionBombardement->setEnabled(true);
        factionSonar->setEnabled(true);
    }
}

void Lobby::updatePlayer() {
    clearPlayerLayout();

    QFont font("Arial", 14);

    int counter = 0;
    for (auto playerName : _view->getUserInGame(_session_id)) {
        QLabel *playerNameLabel = new QLabel(QString::fromStdString(playerName + (counter > 1 ? " (Spectator)" : "")));
        playerNameLabel->setFont(font);
        playerNameLabel->setStyleSheet("color: blue;");
        scrollLayoutPlayer->addWidget(playerNameLabel);
        scrollLayoutPlayer->addSpacing(20);

        if (playerName == _my_username && counter > 1) {
            _spectator_mode = true;
        }
        counter++;
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
        warning->setText("Not enough Player !");
    } else if (!_faction_chosen && _commander_mode) {
        warning->setText("No Faction Chosen !");
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

bool Lobby::isCommanderMode() const {
    return _commander_mode;
}

std::string Lobby::getSessionId() const {
    return _session_id;
}

int Lobby::getSelectedFaction() const {
    return _selected_faction;
}

bool Lobby::isSpectatorMode() const {
    return _spectator_mode;
}