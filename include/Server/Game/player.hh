#pragma once

#include <memory>

#include "connected_user.hh"

/*
 * Joueur en jeu
*/
class Player {
public:
  Player();

private:
    std::shared_ptr<ConnectedUser> _user;
};