#pragma once

#include "../controller.hh"
#include "../../common/message.hh"
#include <vector>
#include <string>

class ChatController : public Controller {
public:
    void sendMsgToServer(const Message& message);
    std::vector<Message> getMessages();

private:
    std::vector<Message> messages;
};
