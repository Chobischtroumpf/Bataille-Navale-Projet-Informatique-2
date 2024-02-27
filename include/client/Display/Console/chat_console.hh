#pragma once

#include <memory>
#include <string>
#include "../../Views/chat_view.hh"
#include "../../Controllers/chat_controller.hh"
#include "../console.hh"
#include "../../../common/message.hh"


class ChatConsole : public Console {
public:
    ChatConsole(const std::string& currentUser, const std::string& selectedUser);
    ReturnInput run();

private:
    std::shared_ptr<ChatView> _view;
    std::shared_ptr<ChatController> _controller;
    std::string currentUser; // (moi)
    std::string selectedUser; // L'ami
    void displayMessage(const std::vector<Message>& messages, std::string selectedUser);
    void diviserEnLignes(std::string& text, std::size_t maxLineLength, std::size_t senderNameLength);
};