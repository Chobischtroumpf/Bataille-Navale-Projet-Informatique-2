#pragma once

#include <memory>
#include <string>
#include "../../Views/chat_view.hh"
#include "../../Controllers/chat_controller.hh"
#include "../console.hh"


class ChatConsole {
public:
    ChatConsole(const std::string& currentUser, const std::string& selectedUser);
    void run();

private:
    std::shared_ptr<ChatView> _view;
    std::shared_ptr<ChatController> _controller;
    std::string currentUser; // (moi)
    std::string selectedUser; // L'ami
};