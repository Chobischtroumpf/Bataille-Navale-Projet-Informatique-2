#pragma once

#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "console.hh"
#include "game_player_controller.hh"
#include "game_player_view.hh"

class GamePlayerConsole : public Console {
private:
  std::shared_ptr<GamePlayerView> _view;
  std::shared_ptr<GamePlayerController> _controller;
  int _friendlist_position = 4;
  void displayTitle();
  void displayPlayer();
  void displayPlayer(int p1);
  void displayPlayer(int p1, int p2);
  void displayOptions(int mode);

public:
  GamePlayerConsole(std::shared_ptr<GamePlayerView> view);
  void display() override;
  void displayError() override;
  void update() override;
  ReturnInput handleInput() override;
};