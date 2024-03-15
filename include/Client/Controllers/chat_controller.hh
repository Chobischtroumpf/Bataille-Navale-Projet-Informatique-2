#pragma once


#include <string>
#include <memory>
#include <future>


#include "controller.hh"
#include "game_client.hh"
#include "message.hh"


class ChatController : public Controller {
public:
    void sendMsgToServer(const std::string& destination , const std::string& message);
    explicit ChatController(std::shared_ptr<GameClient> gameClients);


private:
    std::shared_ptr<GameClient> gameClient;
};
