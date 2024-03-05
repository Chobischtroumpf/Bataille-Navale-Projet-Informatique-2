#pragma once

#include <vector>

#include "controller.hh"
#include "message.hh"

class ChatController : public Controller {
public:
    void send_msg_to_server(const Message& message);
    std::vector<Message> get_messages();

private:
    std::vector<Message> messages_;
};
