#include "chat_out_game_gui.hh"

ChatOut::ChatOut(const std::string &destination, std::shared_ptr<GameClient> gameClient) 
: _destination(destination) {
    _view = std::make_shared<ChatView>(gameClient);
    _controller = std::make_shared<ChatController>(gameClient);

    setWindowTitle("Chat");

    // Champ pour saisir les messages
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Write a message...");

    // Connexion pour envoyer le message en appuyant sur Entrée
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatOut::sendMessage);

    // Bouton pour envoyer les messages
    sendButton = new QPushButton("Send", this);
    connect(sendButton, &QPushButton::clicked, this, &ChatOut::sendMessage);


    messageDisplay = new QTextEdit(this);
    messageDisplay->setReadOnly(true);

    // Bouton et connexion pour le retour au menu
    backToMenu = new QPushButton("Leave the chat", this);
    connect(backToMenu, &QPushButton::clicked, this, &ChatOut::onBackToMenuButtonClicked);

    // Mise en place du timer pour actualiser les messages
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, [this]() {
        updateMessagesDisplay();
    });
    refreshTimer->start(1000); // Actualise toutes les 5 secondes

    // Organisation de la disposition des widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(messageDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    layout->addWidget(backToMenu);
}


void ChatOut::onBackToMenuButtonClicked() {
    //emit goBackToMenu();
    this->close();
}

void ChatOut::updateMessagesDisplay() {
    std::vector<Message> messages = _view->getMsgFromServer(_destination);
    messageDisplay->clear();  //sinn recharge +fois les mm msg 
    for (const auto& message : messages) {
        messageDisplay->append(QString::fromStdString(message.getSender() + ": " + message.getText()));
    }
}

void ChatOut::sendMessage() {
    std::string messageText = messageInput->text().toStdString();
    if (!messageText.empty()) {
        _controller->sendMsgToServer(_destination, messageText); 
        messageInput->clear(); 
    }
}