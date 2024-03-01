#pragma once

#include "../controller.hh"

class MainMenuController : public Controller {
public:
  bool createGame(int mode, int friend_id);
  void joinGame();
  void manageProfile();
  void quit();
};