#pragma once

#include "../controller.hh"

class GamePlayerController : public Controller {
public:
  void chooseRandom();
  void choosePlayer();
  void rechoosePlayer();
  void refresh();
  void next();
  void quit();
};