#ifndef CHATCONSOLE_H
#define CHATCONSOLE_H

#include <iostream>
#include <memory>
#include <string>

#include "ChatController.h"
#include "ChatView.h"

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


#endif // CHATCONSOLE_H
