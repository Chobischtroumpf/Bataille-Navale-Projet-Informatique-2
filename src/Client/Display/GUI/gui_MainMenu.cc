#include "gui_MainMenu.hh"
#include <qt6/QtWidgets/qpushbutton.h>

MainMenu::MainMenu(std::shared_ptr<GameClient> gameClient) {

    // View and controller
    _view = std::make_shared<MainMenuView>(gameClient);
    _controller = std::make_shared<MainMenuController>(gameClient);

    // Initialisation du refresh timer pour update les amis
    timerFriends = new QTimer(this);
    connect(timerFriends, &QTimer::timeout, this, &MainMenu::updateFriends);
    timerFriends->setInterval(10000); // 10 secondes
    timerFriends->start();

    // Gui components initialization
	setWindowTitle("Main Menu");

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

    // Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    // Layout pour la liste d'amis (zone de scroll à la verticale)
    QHBoxLayout *friendsLayout = new QHBoxLayout();
    mainLayout->addLayout(friendsLayout);

    // Layout pour les boutons (zone à l'horizontale)
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);


    // QPushButton pour appuyer sur un bouton
    creatGame = new HighlightButton("Jouer", this);
    creatGame->setStyleSheet("HighlightButton { color : red; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::clicked, this, &MainMenu::onCreatGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(creatGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    addFriend = new HighlightButton("Ajouter un ami", this);
    addFriend->setStyleSheet("QPushButton { color : blue; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::clicked, this, &MainMenu::onAddFriendButtonClicked); // Le bouton se cache une fois cliqué
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(addFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);
/*
    chatWithAFriend = new HighlightButton("Chat avec un ami", this);
    chatWithAFriend->setStyleSheet("QPushButton { color : yellow; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(chatWithAFriend), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);
*/
    joinGame = new HighlightButton("Rejoindre une partie", this);
    joinGame->setStyleSheet("QPushButton { color : green; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::clicked, this, &MainMenu::onJoinGameButtonClicked);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(joinGame), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    logOut = new HighlightButton("Se déconnecter", this);
    logOut->setStyleSheet("QPushButton { color : gray; padding: 10px;}");
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::clicked, this, &MainMenu::onLogOutButtonClicked);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseEntered, this, &MainMenu::mouseOnButton);
    connect(dynamic_cast<HighlightButton*>(logOut), &HighlightButton::mouseLeft, this, &MainMenu::mouseLeftButton);

    // Initialisation des lineEdits
    friendNameLineEdit = new QLineEdit();
    connect(friendNameLineEdit, &QLineEdit::returnPressed, [this] {
        onFriendLineEditReturnPressed();
    });
    friendNameLineEdit->hide(); // Caché par défaut

    // Disposition des objets dans le buttonLayout
    buttonLayout->addWidget(creatGame);
    buttonLayout->addWidget(addFriend);
    buttonLayout->addWidget(friendNameLineEdit);
    buttonLayout->addWidget(chatWithAFriend);
    buttonLayout->addWidget(joinGame);
    buttonLayout->addWidget(logOut);

    // Disposition des objets dans le friendsLayout
    friendsLayout->addWidget(scrollAreaFriends, 1);
    friendsLayout->setAlignment(Qt::AlignTop);

    updateFriends(); // Premier affichage des amis avant un refresh toutes les 10 secondes
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

void MainMenu::onChatWithAFriendButtonClicked(const QString &destination) {
    std::string destinationStd = destination.toStdString();
    emit startChat(destinationStd);
    // Utilisez destinationStd si nécessaire pour des opérations qui requièrent std::string
    // this->close(); // Décommentez si vous souhaitez fermer cette fenêtre après avoir émis le signal
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
        friendButton->setFlat(true);
        friendButton->setStyleSheet("QPushButton { color : green; text-align: left; font-weight: bold;}");
        friendButton->setCursor(Qt::PointingHandCursor);
        scrollLayoutFriends->addWidget(friendButton);
        scrollLayoutFriends->addSpacing(20);

        // passe le nom comme paramètre
        connect(friendButton, &QPushButton::clicked, [this, QStr_name]() {
            this->onChatWithAFriendButtonClicked(QStr_name);
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
