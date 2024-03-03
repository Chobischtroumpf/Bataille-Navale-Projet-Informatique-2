#pragma once

#include "../controller.hh"
#include "../../common/message.hh"
#include <vector>
#include <string>

class ChatController : public Controller {
public:
    void send_msg_to_server(const Message& message);
    std::vector<Message> get_messages();

private:
    std::vector<Message> messages;
};
