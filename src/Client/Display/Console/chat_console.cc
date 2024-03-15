#include "chat_console.hh"
#include "chat_controller.hh"

ChatConsole::ChatConsole(const std::string& sender, const std::string& destination, const std::shared_ptr<GameClient> client)
    : sender(sender), destination(destination), _gameClient(client) {
    _view = std::make_shared<ChatView>();
    _controller = std::make_shared<ChatController>();
}

ReturnInput ChatConsole::handleInput() {
    std::string input;
    std::cout <<"Type './exit' to end chat session\n";
	std::cout<<"Type your message : ";
    while (true) {
        std::getline(std::cin, input);
        if (input == "./exit") {
            std::cout << "Exiting chat...\n";
            return {ReturnInput::MAIN_MENU,""};
        }

		_controller->sendMsgToServer(Message(input, sender,destination));// IMPLEMENTATION POUR TEST le driver, il faudra enlever le lien entre controller et view!
        auto messages = _view->getMessages(_controller->getMessages()); 
		system("clear");

        
        displayMessage(messages,destination);
    }
}

void ChatConsole::displayMessage(const std::vector<Message>& messages, std::string selectedUser){
	std::cout << "Type './exit' to end chat session\n";
    std::cout << "┌─────────────────────────────────────────────┐\n";
    std::cout << "│ Chat with " << "\033[1m\033[31m" << selectedUser << "\033[0m" << "\n";
	std::cout << "├─────────────────────────────────────────────┤\n";
	for (const auto& msg : messages) {
		std::string text = msg.getText();
		diviserEnLignes(text, 45, msg.getSender().length());
		std::cout << "│ \033[4m" << msg.getSender() << "\033[0m: " << text << "\n";
	}
	std::cout << "└─────────────────────────────────────────────┘\n";
	std::cout << "Type your message : ";
}

void ChatConsole::diviserEnLignes(std::string& text, std::size_t maxLineLength, std::size_t senderNameLength) {
    std::size_t start = 0;
    std::size_t end;
	bool isFirstLine = true;

    while (start < text.length()) {
        end = start + (isFirstLine ? maxLineLength - senderNameLength - 2 : maxLineLength);
        if (end >= text.length()) break;
        
        std::size_t spacePos = text.rfind(' ', end);
        if (spacePos == std::string::npos || spacePos <= start) {
            spacePos = text.find(' ', end);
        }

        if (spacePos != std::string::npos) {
            text[spacePos] = '\n';
            if (spacePos + 2 < text.length()) {
                text.insert(spacePos + 1, "│ ");
                end = spacePos + 3;
            }
        }
        start = end;
		isFirstLine = false;
	}
}
