#pragma once

#include "../controller.hh"
#include "../../common/message.hh"

class ChatController : public Controller {
public:
    NotImplementedError get_messages();
    void send_msg_to_server(Message message);
    std::string get_msg_from_server();

private:

};