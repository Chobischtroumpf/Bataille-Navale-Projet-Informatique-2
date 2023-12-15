#pragma once

#include "server.hh"
#include "../common/message.hh"

class ChatServer : public Server {
public:
  Message getNewMessage();
  void broadcast();
};