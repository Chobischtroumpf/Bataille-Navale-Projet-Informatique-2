#pragma once

#include "console.hh"
#include "display.hh"
#include "game_client.hh"
#include <iostream>
#include <memory>
#include <string>

class GameSettingConsole : public Console {
private:
  std::shared_ptr<GameClient> gameClient;
  void displayTitle();
  void displayParameter();
  void displayParameter(std::string game_name);
  void displayParameter(std::string game_name, bool game_mode_classic);
  void displayParameter(std::string game_name, bool game_mode_classic,
                        int time_per_turn);
  void displayParameter(std::string game_name, bool game_mode_classic,
                        int time_per_turn, int time_per_game);
  void displayParameter(std::string game_name, bool game_mode_classic,
                        int time_per_turn, int time_per_game, int time_game);
  void displayParameter(std::string game_name, bool game_mode_classic,
                        int time_per_turn, int time_per_game, int time_game,
                        bool spectator_allowed);
  void displayOptions(int mode);

public:
  GameSettingConsole(std::shared_ptr<GameClient> gameClient);
  void display() override;
  void displayError() override;
  void update() override;
  ReturnInput handleInput() override;
};