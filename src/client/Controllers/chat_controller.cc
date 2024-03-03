#include <iostream>
#include "../../../include/client/Controllers/chat_controller.hh"

void ChatController::send_msg_to_server(const Message& message) {
	// Ajout code pour envoyer Message vers le serveur
    messages.push_back(message);
}

std::vector<Message> ChatController::get_messages() {
    return messages;
}

