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
  short _current_option = 0;
  void displayOptions(int mode);
  // void startGame();
  void addPlayer();

public:
  explicit LobbyConsole(const std::string &sessionId,
                        std::shared_ptr<GameClient> client);

  void displayFriends();
  void display() override;
  void displayError() override {}
  void update() override {}
  ReturnInput handleInput() override;
};
