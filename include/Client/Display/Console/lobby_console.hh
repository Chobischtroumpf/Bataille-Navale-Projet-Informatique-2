#pragma once

#include <memory>

#include "console.hh"
#include "display.hh"
#include "game_client.hh"
#include "lobby_controller.hh"
#include "lobby_view.hh"
#include "game_setting_console.hh"

class LobbyConsole : public Console {
private:
  std::vector<std::string> _players_in_lobby;
  std::string _session_id;
  std::shared_ptr<GameClient> _game_client;
  std::shared_ptr<LobbyView> _view;
  std::shared_ptr<LobbyController> _controller;
  bool _admin;
  short _current_option = 0; // > 0 main menu, > 1 invite a player, > 2 chose a faction
  int _width = 70;
  int _selected_faction = 0;
  bool _valid_input = true;
  std::array<std::string, 3> _factions = {"Faction Bombardement 💣", "Faction Sonar 📡", "Faction Mines 💥"};
  std::vector<std::string> _options_commander = {"Add Player to Lobby ⌨", "Change faction 🚢", "Start Game ⚑", "Refresh Player List 🔄", "Back to the Main Menu ☝"};
  std::vector<std::string> _options_classic = {"Add Player to Lobby ⌨", "Start Game ⚑", "Refresh Player List 🔄", "Back to the Main Menu ☝"};
  std::vector<std::string>& _options = _options_classic;
  

  // Game parameters
  int _max_players;
  std::string _game_name;
  bool _commander_mode;

  void displayOptions(int mode);
  // void startGame();
  void addPlayer(const std::string& playerName);

  ReturnInput handleStartGame();
  ReturnInput handleInvitePlayer();
  ReturnInput handleChoseFaction(int faction);
  ReturnInput backToMainMenu();
  ReturnInput refresh();
  ReturnInput GoToInvitePlayer();

public:
  explicit LobbyConsole(const std::string &sessionId,
                        std::shared_ptr<GameClient> client, bool admin = false);

  void displayFriends();
  void displayFactions();
  void display() override;
  void displayError() override {}
  void update() override {}
  void wait();
  void loadParameters(std::shared_ptr<GameSettingConsole> gameSettingConsole);
  ReturnInput handleInput() override;

  bool isCommanderMode() const;
  int getFaction() const;
};
