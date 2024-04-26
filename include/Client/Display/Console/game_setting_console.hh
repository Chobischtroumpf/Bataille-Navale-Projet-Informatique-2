#pragma once

#include "console.hh"
#include "display.hh"
#include "game_client.hh"
#include <iostream>
#include <memory>
#include <string>
#include <optional>

class GameSettingConsole : public Console {
private:
  std::shared_ptr<GameClient> gameClient;
  void displayTitle();
  void displayParameter();
  void displayOptions(int mode);
  std::optional<bool> _commander_mode = std::nullopt;
  std::optional<bool> _spectator_allowed = std::nullopt;
  std::string _game_name = "";
  std::optional<int> _time_per_turn = std::nullopt;
  std::optional<int> _time_per_game = std::nullopt;
  std::optional<bool> _timer_mode = std::nullopt;
  int _option_counter = 0;
  bool _valid_input = true;

  std::array<std::function<bool(std::string)>, 7> _validators{
      [](std::string input) -> bool {
        for (char &c : input) {
          if (!isalnum(c)) {
            return false;
          }
        }
        return input.size() > 0;
      },
      [](std::string input) -> bool {
        return input.size() == 1 && (input[0] == '1' || input[0] == '2');
      },
      [](std::string input) -> bool {
        return input.size() == 1 && (input[0] == '1' || input[0] == '2');
      },
      [](std::string input) -> bool {
        return input.size() > 0 &&
                std::find_if(input.begin(), input.end(),
                            [](unsigned char c) { return !std::isdigit(c); }) == input.end() &&
                std::stoi(input) >= 30 && std::stoi(input) < 61;
      },
      [](std::string input) -> bool {
          return input.size() > 0 &&
                  std::find_if(input.begin(), input.end(),
                              [](unsigned char c) { return !std::isdigit(c); }) == input.end() &&
                  std::stoi(input) > 60 && std::stoi(input) < 1001;
      },
      [](std::string input) -> bool { return input.size() == 1 && (input[0] == '1' || input[0] == '2');},
      [](std::string input) -> bool { return input.size() == 1 && (input[0] == '1' || input[0] == '2');}
  };

public:
  GameSettingConsole(std::shared_ptr<GameClient> gameClient);
  void display() override;
  void displayError() override;
  void update() override;
  ReturnInput handleInput() override;

  // Getters
  bool isCommanderMode() const;
  bool isClassicTimer() const;
  bool isSpectatorAllowed() const;
  std::string getGameName() const;
  int getTimePerTurn() const;
  int getTimePerGame() const;
};