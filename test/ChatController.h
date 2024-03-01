#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <vector>
#include <string>

#include "messages.hh"

class ChatController {
public:
    void send_msg_to_server(const Message& message);
    std::vector<Message> get_messages();

private:
    std::vector<Message> messages;
};

#endif // CHATCONTROLLER_H
