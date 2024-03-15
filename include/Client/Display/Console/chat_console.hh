#pragma once

#include <memory>
#include <string>
#include <iostream>

#include "chat_view.hh"
#include "chat_controller.hh"
#include "console.hh"
#include "message.hh"
#include "game_client.hh"


class ChatConsole : public Console {
public:
    ChatConsole(const std::string& sender, const std::string& destination, const std::shared_ptr<GameClient> client);
    virtual void display() override {}
    virtual void displayError() override {}
    virtual void update() override {}
    ReturnInput handleInput() override;

private:
    std::shared_ptr<ChatView> _view;
    std::shared_ptr<ChatController> _controller;
    std::shared_ptr<GameClient> _gameClient;
    std::string sender; 
    std::string destination;
    void displayMessage(const std::vector<Message>& messages, std::string selectedUser);
    void diviserEnLignes(std::string& text, std::size_t maxLineLength, std::size_t senderNameLength);
};