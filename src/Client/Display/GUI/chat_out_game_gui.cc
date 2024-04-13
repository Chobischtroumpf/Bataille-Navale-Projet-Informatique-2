#include "chat_out_game_gui.hh"

ChatOut::ChatOut(const std::string &destination, std::shared_ptr<GameClient> gameClient) : _destination(destination) {
    _view = std::make_shared<ChatView>(gameClient);
    _controller = std::make_shared<ChatController>(gameClient);

    setWindowTitle("Chat");

    backToMenu = new QPushButton("Go back to the main menu");
    connect(backToMenu, &QPushButton::clicked, this, &ChatOut::onBackToMenuButtonClicked);

    // Disposition des différents objets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(backToMenu);

}

void ChatOut::onBackToMenuButtonClicked() {
    emit goBackToMenu();
    this->close();
}
