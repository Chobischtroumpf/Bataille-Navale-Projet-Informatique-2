#include "gui_MainMenu.hh"
#include <qt6/QtWidgets/qpushbutton.h>

MainMenu::MainMenu(std::shared_ptr<GameClient> gameClient) {
    /*
     * Les layouts sont gérés automatiquement par Qt et sont supprimés à la fin de l'exécution,
     * ainsi que tous les widgets qui leurs sont associés.
     */

    // View and controller
    _view = std::make_shared<MainMenuView>(gameClient);
    _controller = std::make_shared<MainMenuController>(gameClient);

    // Initialisation du refresh timer pour update les amis
    timerFriends = new QTimer(this);
    connect(timerFriends, &QTimer::timeout, this, &MainMenu::updateFriends);
    timerFriends->setInterval(10000); // 10 secondes
    timerFriends->start();

    frameCounter = 0;

    // Initialisation du refresh timer pour update les notifications
    timerNotifications = new QTimer(this);
    connect(timerNotifications, &QTimer::timeout, this, &MainMenu::updateNotifications);
    timerNotifications->setInterval(1000); // 1 secondes
    timerNotifications->start();

    // Initialisation du refresh timer pour update le background
    timerBackground = new QTimer(this);
    connect(timerBackground, &QTimer::timeout, this, &MainMenu::updateBackground);
    timerBackground->setInterval(1000/60); // 60 fois par seconde
    timerBackground->start();

    // Gui components initialization
	setWindowTitle("Main Menu");
    //setAttribute(Qt::WA_TranslucentBackground); // Funny thing to do ;)
    resize(800, 650);

    // QScrollArea pour la liste d'amis
    QScrollArea *scrollAreaFriends = new QScrollArea(this);
    QWidget *scrollWidgetFriends = new QWidget(scrollAreaFriends);
    scrollLayoutFriends = new QVBoxLayout(scrollWidgetFriends);
    scrollWidgetFriends->setLayout(scrollLayoutFriends);
    scrollAreaFriends->setWidgetResizable(true);
    scrollAreaFriends->setWidget(scrollWidgetFriends);
    scrollAreaFriends->setFixedWidth(200);
    scrollAreaFriends->setFixedHeight(300);
    scrollAreaFriends->setStyleSheet("QScrollArea {background-color: white; border: 2px solid black; border-radius: 5px;}");


    // QScrollArea pour les notifications
    QScrollArea *scrollAreaNotifications = new QScrollArea(this);
    QWidget *scrollWidgetNotifications = new QWidget(scrollAreaNotifications);
    scrollLayoutNotifications = new QVBoxLayout(scrollWidgetNotifications);
    scrollWidgetNotifications->setLayout(scrollLayoutNotifications);
    scrollAreaNotifications->setWidgetResizable(true);
    scrollAreaNotifications->setWidget(scrollWidgetNotifications);
    scrollAreaNotifications->setFixedWidth(600);
    scrollAreaNotifications->setFixedHeight(300);
    scrollAreaNotifications->setStyleSheet("QScrollArea { border: 2px solid black; border-radius: 5px;}");

    // Layout principal (dispose ses layouts enfants à la verticale)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    setLayout(mainLayout);

    // Layout pour la liste d'amis et les notifications
    QHBoxLayout *friendsAndNotificationsLayout = new QHBoxLayout();
    mainLayout->addLayout(friendsAndNotificationsLayout);

    // Layout pour les boutons (zone à l'horizontale)
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);


    // QPushButton pour appuyer sur un bouton
    creatGame = new HighlightButton("Jouer", this); creatGame->setFixedWidth(400);
    creatGame->setStyleSheet("HighlightButton { color : white; padding: 10px; border-image: url(src/Common/Images/submarine.png); font-weight: bold;}");
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::clicked, this, &MainMenu::onCreatGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    addFriend = new HighlightButton("Ajouter un ami", this); addFriend->setFixedWidth(400);
    addFriend->setStyleSheet("QPushButton { color : white; padding: 10px; background-color: #333333;}");
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::clicked, this, &MainMenu::onAddFriendButtonClicked); // Le bouton se cache une fois cliqué
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    chatWithAFriend = new HighlightButton("Chat avec un ami", this); chatWithAFriend->setFixedWidth(400);
    chatWithAFriend->setStyleSheet("QPushButton { color : white; padding: 10px; background-color: #333333;}");
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    joinGame = new HighlightButton("Rejoindre une partie", this); joinGame->setFixedWidth(400);
    joinGame->setStyleSheet("QPushButton { color : white; padding: 10px;  background-color: #333333;}");
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::clicked, this, &MainMenu::onJoinGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);


    review = new HighlightButton("Visualiser une partie", this); review->setFixedWidth(400);
    review->setStyleSheet("QPushButton { color : white; padding: 10px; background-color: #333333;}");
    connect(dynamic_cast<HighlightButton*>(review), &HighlightButton::clicked, this, &MainMenu::onReviewButtonClicked);
    connect(dynamic_cast<HighlightButton*>(review), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(review), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    logOut = new HighlightButton("Se déconnecter", this); logOut->setFixedWidth(400);
    logOut->setStyleSheet("QPushButton { color : gray; padding: 10px; background-color: #333333;}");
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::clicked, this, &MainMenu::onLogOutButtonClicked);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    // Initialisation des lineEdits
    friendNameLineEdit = new QLineEdit(); friendNameLineEdit->setFixedWidth(400);
    connect(friendNameLineEdit, &QLineEdit::returnPressed, [this] {
        onFriendLineEditReturnPressed();
    });
    friendNameLineEdit->setPlaceholderText("Entrez un nom d'utilisateur");
    friendNameLineEdit->hide(); // Caché par défaut


    chatFriendLineEdit = new QLineEdit(); chatFriendLineEdit->setFixedWidth(400);
    connect(chatFriendLineEdit, &QLineEdit::returnPressed, [this] {
    onChatFriendLineEditReturnPressed();
    });
    chatFriendLineEdit->setPlaceholderText("Entrez un nom d'ami");
    chatFriendLineEdit->hide();


    joinGameLineEdit = new QLineEdit(); joinGameLineEdit->setFixedWidth(400);
    connect(joinGameLineEdit, &QLineEdit::returnPressed, [this] {
        onJoinGameLineEditReturnPressed();
    });
    joinGameLineEdit->setPlaceholderText("Entrez un gameID");
    joinGameLineEdit->hide();

    // Disposition des objets dans le buttonLayout
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(creatGame);
    buttonLayout->addWidget(addFriend);
    buttonLayout->addWidget(friendNameLineEdit);
    buttonLayout->addWidget(chatFriendLineEdit);
    buttonLayout->addWidget(chatWithAFriend);
    buttonLayout->addWidget(joinGameLineEdit);
    buttonLayout->addWidget(joinGame);
    buttonLayout->addWidget(review);
    buttonLayout->addWidget(logOut);

    // Disposition des objets dans le friendsLayout
    friendsAndNotificationsLayout->addWidget(scrollAreaFriends, 1);
    friendsAndNotificationsLayout->setAlignment(Qt::AlignTop);
    friendsAndNotificationsLayout->addWidget(scrollAreaNotifications, 1);

    updateFriends(); // Premier affichage des amis avant un refresh toutes les 10 secondes
    updateNotifications(); // IDEM
}


void MainMenu::onCreatGameButtonClicked() {
    emit startGameSetting();
    this->close();
}

void MainMenu::onReviewButtonClicked() {
    emit startReview();
    this->close();
}


void MainMenu::onAddFriendButtonClicked() {
    addFriend->hide(); friendNameLineEdit->show();
}

void MainMenu::onFriendLineEditReturnPressed() {

    // Possibilité d'entrer un nom d'utilisateur
    friendNameLineEdit->show();
    QString friendName = friendNameLineEdit->text();

    qDebug() << "Entrée utilisateur :" << friendName; // Debug

    // Nettoyage et widget caché
    friendNameLineEdit->clear();
    friendNameLineEdit->hide();

    // On peut à nouveau demander la requête pour ajouter un ami
    addFriend->show();

    // Requête au serveur
    std::string str_friendName = friendName.toStdString();
    _controller->addFriend(str_friendName);
    updateFriends();
}

MainMenu::~MainMenu() {delete timerFriends; delete timerNotifications; delete timerBackground;}

void MainMenu::onChatWithAFriendButtonClicked() {
    chatWithAFriend->hide(); // Cache le bouton "Chat avec un ami"
    chatFriendLineEdit->show();
    //std::string destinationStd = destination.toStdString();
    //emit startChat(destinationStd);
}
void MainMenu::onChatFriendLineEditReturnPressed() {
    chatFriendLineEdit->show();
    QString friendName = chatFriendLineEdit->text();
    qDebug() << "Tentative de chat avec :" << friendName; // Debug

    // Nettoyage et widget caché
    chatFriendLineEdit->clear();
    chatFriendLineEdit->hide();

    chatWithAFriend->show();

    std::string str_friendName = friendName.toStdString();
    emit startChat(str_friendName);
}

void MainMenu::onJoinGameLineEditReturnPressed() {
    joinGameLineEdit->show();
    QString gameID = joinGameLineEdit->text();
    qDebug() << "Tentative de connexion avec gameID :" << gameID; // Debug

    // Nettoyage et widget caché
    joinGameLineEdit->clear();
    joinGameLineEdit->hide();

    joinGame->show();

    std::string str_gameID = gameID.toStdString();

    if (_controller->joinGame(str_gameID)) {
        this->close();
        emit startLobby(str_gameID);
    }
}

void MainMenu::onFriendNameButtonClicked(const QString &destination){
    std::string destinationStd = destination.toStdString();
    emit startChat(destinationStd);
}

void MainMenu::onNotificationButtonClicked(const QString &info) {
    std::string std_info = info.toStdString();

    size_t pos_id = std_info.find("sessionID: ");
    size_t pos_new_friend_added = std_info.find("You've just added ");
    size_t pos_just_been_added = std_info.find(" added you as a friend!");


    // Notification d'invitation à une partie
    if (pos_id != std::string::npos) {
        pos_id += 11; // Longueur de "sessionID: "
        std::string sessionId = std_info.substr(pos_id, std_info.length()); // Longueur d'un sessionID
        std::cout << sessionId << std::endl;
        if (_controller->joinGame(sessionId)) {
            this->close();
            emit startLobby(sessionId);
        }
    }

    // Notification d'ajout en ami
    else if (pos_new_friend_added != std::string::npos) {
        pos_new_friend_added += std::string("You've just added ").length();

        // Recherche de la position de " as a friend"
        size_t endPos = std_info.find(" as a friend", pos_new_friend_added);

        if (endPos != std::string::npos) {
            // Récupération de l'username et démarrage du chat
            std::string username = std_info.substr(pos_new_friend_added, endPos - pos_new_friend_added);
            emit startChat(username);
        }
    }

    // Notification when you've just been added
    else if (pos_just_been_added != std::string::npos) {
        size_t startPos = std_info.find(" added you as a friend!");

        if (startPos != std::string::npos) {
            std::string username = std_info.substr(0, startPos);
            username = username.substr(4, username.length());
            emit startChat(username);
        }
    }
}

void MainMenu::onJoinGameButtonClicked() {
    joinGame->hide();
    joinGameLineEdit->show();
}

void MainMenu::onLogOutButtonClicked() {
    emit userDisconnection();
    this->close();
}

void MainMenu::updateFriends() {
    clearFriendsLayout();

    for (auto friendInfo : _view->getFriends()) {
        std::string name = get<0>(friendInfo);
        QString QStr_name = QString::fromStdString(name);
        QPushButton *friendButton = new QPushButton(QStr_name);
        friendButton->setFlat(true); // Retire le contour du bouton
        friendButton->setStyleSheet("QPushButton { color : green; "
                                    "text-align: left;"
                                    "font-weight: bold;}"); // Change la couleur du texte en bleu
        friendButton->setCursor(Qt::PointingHandCursor); // Change le curseur lorsqu'il passe au-dessus du QPushButton
        scrollLayoutFriends->addWidget(friendButton);
        scrollLayoutFriends->addSpacing(20);

        // passe le nom comme paramètre
        connect(friendButton, &QPushButton::clicked, [this, QStr_name]() {
            this->onFriendNameButtonClicked(QStr_name);
        });
    }
}

void MainMenu::updateNotifications() {

    clearNotificationsLayout();

    for (auto notification : _view->getNotifications()) {
        notification.insert(0, "◈ ");
        QString notification_content = QString::fromStdString(notification);
        QPushButton *NotificationButton = new QPushButton(notification_content);
        NotificationButton->setFlat(true); // Retire le contour du bouton
        NotificationButton->setStyleSheet("QPushButton { color : orange; "
                                          "text-align: left;"
                                          "font-weight: bold;}");
        NotificationButton->setCursor(Qt::PointingHandCursor); // Change le curseur lorsqu'il passe au-dessus du QPushButton
        scrollLayoutNotifications->addWidget(NotificationButton);
        scrollLayoutNotifications->addSpacing(20);

        connect(NotificationButton, &QPushButton::clicked, [this, notification_content]() {
            this->onNotificationButtonClicked(notification_content);
        });
    }
}

void MainMenu::clearFriendsLayout() {
    QLayoutItem *item;
    while ((item = scrollLayoutFriends->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
}

void MainMenu::clearNotificationsLayout() {
    QLayoutItem *item;
    while ((item = scrollLayoutNotifications->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
}

void MainMenu::mouseOnButton() {
    // augmente la taille du bouton lorsque la souris passe dessus
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString currentStyleSheet = button->styleSheet();
        QRegularExpression re("padding:\\s*\\d+px;");
        QString newStyleSheet = currentStyleSheet.replace(re, "padding: 15px;");
        button->setStyleSheet(newStyleSheet);
    }
}

void MainMenu::mouseLeftButton() {
    // rétabli l'état du bouton quand la souris n'est plus dessus
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString currentStyleSheet = button->styleSheet();
        QRegularExpression re("padding:\\s*\\d+px;");
        QString newStyleSheet = currentStyleSheet.replace(re, "padding: 10px;");
        button->setStyleSheet(newStyleSheet);
    }
}

// HighlightButton
HighlightButton::HighlightButton(QString name, QWidget *parent) : QPushButton(name, parent) {}

void HighlightButton::enterEvent(QEnterEvent *event) {
    QPushButton::enterEvent(event);
    emit mouseEntered();
}

void HighlightButton::leaveEvent(QEvent *event) {
    QPushButton::leaveEvent(event);
    emit mouseLeft();
}

void MainMenu::updateBackground() {
    update();
}


void MainMenu::paintEvent(QPaintEvent *event) {
    frameCounter = (frameCounter + 1) % 60; // Reset frameCounter after reaching 60

    QWidget::paintEvent(event);
    QPainter painter(this);
    QPixmap backgroundImage;

    if (frameCounter < 15) {
        backgroundImage = QPixmap("src/Common/Images/main_menu_background_1.png");
    }
    else if (frameCounter < 30) {
        backgroundImage = QPixmap("src/Common/Images/main_menu_background_2.png");
    }
    else if (frameCounter < 45) {
        backgroundImage = QPixmap("src/Common/Images/main_menu_background_1.png");
    }
    else {
        backgroundImage = QPixmap("src/Common/Images/main_menu_background_3.png");
    }

    painter.drawPixmap(rect(), backgroundImage);
}
