#pragma once

#include "../controller.hh"
#include "../../../src/common/message.hh"
#include <vector>
#include <string>

class ChatController {
public:
    void send_msg_to_server(const Message& message);
    std::vector<Message> get_messages();

private:
    std::vector<Message> messages;
};
