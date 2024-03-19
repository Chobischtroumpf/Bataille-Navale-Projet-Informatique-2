#pragma once

#include <memory>

#include "console.hh"
#include "display.hh"
#include "game_client.hh"
#include "lobby_controller.hh"
#include "lobby_view.hh"

class LobbyConsole : public Console {
private:
  std::vector<std::string> _players_in_lobby;
  std::string _session_id;
  std::shared_ptr<GameClient> _game_client;
  std::shared_ptr<LobbyView> _view;
  std::shared_ptr<LobbyController> _controller;
  bool _admin;
  short _current_option = 0;

  // Game parameters
  int _max_players;
  std::string _game_name;
  bool _commander_mode;

  void displayOptions(int mode);
  // void startGame();
  void addPlayer(const std::string& playerName);

public:
  explicit LobbyConsole(const std::string &sessionId,
                        std::shared_ptr<GameClient> client, bool admin = false);

  void displayFriends();
  void display() override;
  void displayError() override {}
  void update() override {}
  void wait();
  void loadParameters(const GameSettingConsole &gameSettingConsole);
  ReturnInput handleInput() override;
};
