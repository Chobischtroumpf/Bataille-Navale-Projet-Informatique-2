#include <iostream>

#include "../../../include/client/Views/chat_view.hh"



void ChatView::displayMessage(const std::vector<Message>& messages, std::string selectedUser){
	std::cout << "Type './exit' to end chat session\n";
    std::cout << "┌─────────────────────────────────┐\n";
    std::cout << "│ Chat with " << selectedUser << "\n";
	std::cout << "├─────────────────────────────────┤\n";
	for (const auto& msg : messages) {
		std::cout << "│ " << msg.getSender() << ": " << msg.getText() << "\n";
	}
	std::cout << "└─────────────────────────────────┘\n";
	std::cout << "Type your message : ";
}
