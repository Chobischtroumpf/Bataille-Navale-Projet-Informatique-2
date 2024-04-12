#include "gui_MainMenu.hh"
#include <qt6/QtWidgets/qpushbutton.h>

MainMenu::MainMenu(std::shared_ptr<GameClient> gameClient) : main_menu_controller(gameClient) {

	setWindowTitle("Game");

    resize(800, 600);
    // QPushButton pour appuyer sur un bouton
    creatGame = new QPushButton("Jouer");
    connect(creatGame, &QPushButton::clicked, this, &MainMenu::onCreatGameButtonClicked);

    addFriend = new QPushButton("Ajouter un ami", this);
    connect(addFriend, &QPushButton::clicked, this, &MainMenu::onAddFriendButtonClicked);

    chatWithAFriend = new QPushButton("Chat avec un ami", this);
    connect(chatWithAFriend, &QPushButton::clicked, this, &MainMenu::onChatWithAFriendButtonClicked);

    joinGame = new QPushButton("Rejoindre une partie", this);
    connect(joinGame, &QPushButton::clicked, this, &MainMenu::onJoinGameButtonClicked);

    logOut = new QPushButton("Se déconnecter", this);
    connect(logOut, &QPushButton::clicked, this, &MainMenu::onLogOutButtonClicked);


    // disposition des différents objets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(creatGame);
    layout->addWidget(addFriend);
    layout->addWidget(chatWithAFriend);
    layout->addWidget(joinGame);
    layout->addWidget(logOut);
}


void MainMenu::onCreatGameButtonClicked() {
}

void MainMenu::onAddFriendButtonClicked() {
}

void MainMenu::onChatWithAFriendButtonClicked() {
}

void MainMenu::onJoinGameButtonClicked() {
}

void MainMenu::onLogOutButtonClicked() {
}
