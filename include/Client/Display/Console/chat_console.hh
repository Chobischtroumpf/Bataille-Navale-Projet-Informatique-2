#pragma once


#include <memory>
#include <string>
#include <iostream>


#include "chat_view.hh"
#include "chat_controller.hh"
#include "console.hh"
#include "message.hh"




class ChatConsole : public Console {
public:
    ChatConsole(const std::string& destination, std::shared_ptr<GameClient> gameClient);
    virtual void display() override {}
    virtual void displayError() override {}
    virtual void update() override {}
    ReturnInput handleInput() override;


private:
    //ChatController chatController;
    std::shared_ptr<ChatView> _view;
    std::shared_ptr<ChatController> _controller;
    std::string destination;
    void displayMessage(const std::vector<Message>& messages, std::string selectedUser);
    void diviserEnLignes(std::string& text, std::size_t maxLineLength, std::size_t senderNameLength);
};
