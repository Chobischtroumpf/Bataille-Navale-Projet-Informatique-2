#pragma once

#include "controller.hh"

class GameSettingController : public Controller {
public:
  void chooseGameName();
  void selectGameMode();
  void selectTimePerTurn();
  void selectTimePerGame();
  void selectIfSpectator();
  void goToGamePlayer();
  void quit();
};