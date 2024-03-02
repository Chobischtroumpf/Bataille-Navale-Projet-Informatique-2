#include <iostream>
#include "../../../../include/client/Display/Console/chat_console.hh"
#include "../../../../include/client/Controllers/chat_controller.hh"


ChatConsole::ChatConsole(const std::string& senderUser, const std::string& receiverUser)
    : currentUser(senderUser), selectedUser(receiverUser) {
    _view = std::make_shared<ChatView>();
    _controller = std::make_shared<ChatController>();
}

void ChatConsole::run() {
    std::string input;
    std::cout <<"Type './exit' to end chat session\n";
	std::cout<<"Type your message : ";
    while (true) {
        std::getline(std::cin, input);
        if (input == "./exit") {
            std::cout << "Exiting chat...\n";
            break; // AJOUT méthode changement de fenêtre
        }

		_controller->send_msg_to_server(Message(input, currentUser, selectedUser));
        auto messages = _controller->get_messages(); 
		system("clear");
        _view->displayMessage(messages,selectedUser);
    }
}
