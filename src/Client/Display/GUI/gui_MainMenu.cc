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

    // Initialisation du refresh timer pour update les notifications
    timerNotifications = new QTimer(this);
    connect(timerNotifications, &QTimer::timeout, this, &MainMenu::updateNotifications);
    timerNotifications->setInterval(10000); // 10 secondes
    timerNotifications->start();

    // Gui components initialization
	setWindowTitle("Main Menu");
    //setAttribute(Qt::WA_TranslucentBackground); // Funny thing to do ;)
    resize(800, 600);

    // QScrollArea pour la liste d'amis
    QScrollArea *scrollAreaFriends = new QScrollArea(this);
    QWidget *scrollWidgetFriends = new QWidget(scrollAreaFriends);
    scrollLayoutFriends = new QVBoxLayout(scrollWidgetFriends);
    scrollWidgetFriends->setLayout(scrollLayoutFriends);
    scrollAreaFriends->setWidgetResizable(true);
    scrollAreaFriends->setWidget(scrollWidgetFriends);
    scrollAreaFriends->setFixedWidth(200);
    scrollAreaFriends->setFixedHeight(300);

    // QScrollArea pour les notifications
    QScrollArea *scrollAreaNotifications = new QScrollArea(this);
    QWidget *scrollWidgetNotifications = new QWidget(scrollAreaNotifications);
    scrollLayoutNotifications = new QVBoxLayout(scrollWidgetNotifications);
    scrollWidgetNotifications->setLayout(scrollLayoutNotifications);
    scrollAreaNotifications->setWidgetResizable(true);
    scrollAreaNotifications->setWidget(scrollWidgetNotifications);
    scrollAreaNotifications->setFixedWidth(600);
    scrollAreaNotifications->setFixedHeight(300);

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
    creatGame->setStyleSheet("HighlightButton { color : red; padding: 10px; border-image: url(src/Common/Images/submarine.png);}");
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::clicked, this, &MainMenu::onCreatGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    addFriend = new HighlightButton("Ajouter un ami", this); addFriend->setFixedWidth(400);
    addFriend->setStyleSheet("QPushButton { color : blue; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::clicked, this, &MainMenu::onAddFriendButtonClicked); // Le bouton se cache une fois cliqué
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    chatWithAFriend = new HighlightButton("Chat avec un ami", this); chatWithAFriend->setFixedWidth(400);
    chatWithAFriend->setStyleSheet("QPushButton { color : yellow; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    joinGame = new HighlightButton("Rejoindre une partie", this); joinGame->setFixedWidth(400);
    joinGame->setStyleSheet("QPushButton { color : green; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::clicked, this, &MainMenu::onJoinGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    logOut = new HighlightButton("Se déconnecter", this); logOut->setFixedWidth(400);
    logOut->setStyleSheet("QPushButton { color : gray; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::clicked, this, &MainMenu::onLogOutButtonClicked);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    // Initialisation des lineEdits
    friendNameLineEdit = new QLineEdit(); friendNameLineEdit->setFixedWidth(400);
    connect(friendNameLineEdit, &QLineEdit::returnPressed, [this] {
        onFriendLineEditReturnPressed();
    });
    friendNameLineEdit->hide(); // Caché par défaut

    // Disposition des objets dans le buttonLayout
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(creatGame);
    buttonLayout->addWidget(addFriend);
    buttonLayout->addWidget(friendNameLineEdit);
    buttonLayout->addWidget(chatWithAFriend);
    buttonLayout->addWidget(joinGame);
    buttonLayout->addWidget(logOut);

    // Disposition des objets dans le friendsLayout
    friendsAndNotificationsLayout->addWidget(scrollAreaFriends, 1);
    friendsAndNotificationsLayout->setAlignment(Qt::AlignTop);
    friendsAndNotificationsLayout->addWidget(scrollAreaNotifications, 1);

    updateFriends(); // Premier affichage des amis avant un refresh toutes les 10 secondes
    updateNotifications(); // IDEM
}


void MainMenu::onCreatGameButtonClicked() {
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

MainMenu::~MainMenu() {delete timerFriends;}

void MainMenu::onChatWithAFriendButtonClicked() {
    emit startChat();
    this->close();
}

void MainMenu::onJoinGameButtonClicked() {
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
                                    "font-weight: bold;}");
        friendButton->setCursor(Qt::PointingHandCursor); // Change le curseur lorsqu'il passe au-dessus du QPushButton
        scrollLayoutFriends->addWidget(friendButton);
        scrollLayoutFriends->addSpacing(20);
        connect(friendButton, &QPushButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);
    }

    qDebug() << "Largeur du bouton creatGame :" << creatGame->width();
    qDebug() << "Hauteur du bouton creatGame :" << creatGame->height();
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
        //connect(friendButton, &QPushButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);
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

void MainMenu::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPixmap backgroundImage("src/Common/Images/main_menu_background.png");
    painter.drawPixmap(rect(), backgroundImage);
}

