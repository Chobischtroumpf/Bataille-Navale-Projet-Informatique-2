#pragma once

#include "../controller.hh"

class MainMenuController : public Controller {
public:
  bool createGame(int mode, int friend_id);
  void joinGame(int game_id);
  void addFriend(int friend_id);
  void manageProfile();
  void quit();
};