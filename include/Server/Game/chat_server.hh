#pragma once

#include "server.hh"
#include "message.hh"

/*
 * Gère les messages des utilisateurs
*/
class ChatServer : public Server {
public:
  Message getNewMessage();
  void broadcast();
};