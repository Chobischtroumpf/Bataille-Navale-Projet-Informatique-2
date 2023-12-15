#pragma once

/*
 * Représente un utilisateur connecté
 */
class ConnectedUser {
public:
private:
  unsigned int _user_id;
  int socket_fd;
};