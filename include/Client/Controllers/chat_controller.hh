#pragma once

#include <vector>

#include "controller.hh"
#include "message.hh"

class ChatController : public Controller {
public:
    void sendMsgToServer(const Message& message);
    std::vector<Message> getMessages();

private:
    std::vector<Message> messages_;
};
