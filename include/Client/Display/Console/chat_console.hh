#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "chat_controller.hh"
#include "chat_view.hh"
#include "console.hh"
#include "message.hh"

class ChatConsole : public Console {
private:
  std::shared_ptr<ChatView> _view;
  std::shared_ptr<ChatController> _controller;
  std::string _destination;
  void displayMessage(const std::vector<Message> &messages,
                      std::string selectedUser);
  void diviserEnLignes(std::string &text, std::size_t maxLineLength,
                       std::size_t senderNameLength);

public:
  explicit ChatConsole(const std::string &destination,
                       std::shared_ptr<GameClient> gameClient);
  virtual void display() override {}
  virtual void displayError() override {}
  virtual void update() override {}
  ReturnInput handleInput() override;

};
