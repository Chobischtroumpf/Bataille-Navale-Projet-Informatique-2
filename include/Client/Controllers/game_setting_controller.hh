#pragma once

#include "controller.hh"

class GameSettingController : public Controller {
public:
  void chooseGameName();
  void selectNumberOfBoat();
  void selectIfSpectator();
  void goToGamePlayer();
  void quit();
};