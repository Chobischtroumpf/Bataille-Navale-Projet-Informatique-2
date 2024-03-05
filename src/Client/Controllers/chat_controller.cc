#include <iostream>

#include "chat_controller.hh"

void ChatController::sendMsgToServer(const Message& message) {
	// Ajout code pour envoyer Message vers le serveur
    this->messages_.push_back(message);
}

std::vector<Message> ChatController::getMessages() {
    return this->messages_;
}

