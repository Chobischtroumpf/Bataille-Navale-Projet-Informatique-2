#include "driver.hh"
#include "chat_console.hh"
#include "game_console.hh"
#include "faction_sonar.hh"
#include "lobby_console.hh"
#include "login_console.hh"
#include "faction_mines.hh"
#include "register_console.hh"
#include "main_menu_console.hh"
#include "game_setting_console.hh"
#include "faction_bombardement.hh"
#include "review_console.hh"
#include "review_game_console.hh"

Driver::Driver(DisplayType display_type, std::string server_address) : _display_type{display_type}, _game_client{std::make_shared<GameClient>(server_address)} {}

Driver::~Driver() {}

void Driver::launchApp() {
  displayLoginScreen();
  run();
}

void Driver::run() {
    while (true) {
      _display->display();
      ReturnInput input = _display->handleInput();
      if (_current_screen != input.screen)
      switch (input.screen) {
        case ReturnInput::Screen::MAIN_MENU:
          displayMainMenuScreen();
          break;
        case ReturnInput::Screen::GAME:
          displayGameScreen(input.arg);
          break;
        case ReturnInput::Screen::CHAT:
          displayChatScreen(input.arg);
          break;
        case ReturnInput::Screen::LOBBY:
          if (_current_screen == ReturnInput::Screen::MAIN_MENU)
            displayLobbyScreen(input.arg, false);
          else
            displayLobbyScreen(input.arg, true);
          break;
        case ReturnInput::Screen::LOGIN:
          displayLoginScreen();
          break;
        case ReturnInput::Screen::REGISTER:
          displayRegisterScreen();
          break;
        case ReturnInput::Screen::GAME_CREATION:
          displayGameCreationScreen();
          break;
        case ReturnInput::Screen::REVIEW_MENU:
          displayReviewScreen();
          break;
        case ReturnInput::Screen::REVIEW_GAME:
          displayReviewGameScreen(input.arg);
          break;
      }
    }
}

std::shared_ptr<GameClient> Driver::getClient() {
    return _game_client;
}

void Driver::displayGameScreen(std::string gameId) {
  std::clog << "displayGameScreen" << std::endl;
  if (_display_type == CONSOLE) {
    Player player1 = Player();
    std::clog << "commander_mode" << std::endl;
    bool commander_mode = std::static_pointer_cast<LobbyConsole>(_display)->isCommanderMode();
    bool isSpectator = std::static_pointer_cast<LobbyConsole>(_display)->isSpectatorMode();
    std::clog << "faction" << std::endl;
    int faction =
        std::static_pointer_cast<LobbyConsole>(_display)->getFaction();
    if (commander_mode) {
      switch (faction) {
      case 0:
        player1.setFaction(FactionBombardement());
        break;
      case 1:
        player1.setFaction(FactionSonar());
        break;
      case 2:
        player1.setFaction(FactionMines());
        break;
      }
    } else {
      player1.setFaction(FactionClassique());
    }

    std::shared_ptr<LocalBoardCommander> board = std::make_shared<LocalBoardCommander>(getClient(), player1, commander_mode ? GameMode::COMMANDER : GameMode::CLASSIC, gameId);
    std::shared_ptr<GameController> game_controller = std::make_shared<GameController>(board);
    _display = std::make_shared<GameConsole>(std::cout, std::cin, board, game_controller, getClient(), isSpectator);
    _current_screen = ReturnInput::Screen::GAME;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLoginScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<LoginConsole>(getClient());
    _current_screen = ReturnInput::Screen::LOGIN;
  } else {
  }
}

void Driver::displayRegisterScreen() {
    // Ce display sert-il ?
  if (_display_type == CONSOLE) {
    _display = std::make_shared<RegisterConsole>();
    _current_screen = ReturnInput::Screen::REGISTER;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayMainMenuScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<MainMenuConsole>(getClient());
    _current_screen = ReturnInput::Screen::MAIN_MENU;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayChatScreen(std::string username) {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<ChatConsole>(username, getClient());
    _current_screen = ReturnInput::Screen::CHAT;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayLobbyScreen(std::string gameId, bool admin) {
  if (_display_type == CONSOLE) {
    if (_current_screen == ReturnInput::Screen::GAME_CREATION) {
      std::shared_ptr<LobbyConsole> lobby = std::make_shared<LobbyConsole>(gameId, getClient(), admin, std::static_pointer_cast<GameSettingConsole>(_display));
      _display = std::move(lobby);
      _current_screen = ReturnInput::Screen::LOBBY;
    } else {
      _display = std::make_shared<LobbyConsole>(gameId, getClient(), admin);
      _current_screen = ReturnInput::Screen::LOBBY;
    }
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayGameCreationScreen() {
  if (_display_type == CONSOLE) {
    _display = std::make_shared<GameSettingConsole>(getClient());
    _current_screen = ReturnInput::Screen::GAME_CREATION;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayReviewScreen(){
  if (_display_type == CONSOLE) {
      std::shared_ptr<ReviewController> review_controller = std::make_shared<ReviewController>(getClient());
      _display = std::make_shared<ReviewConsole>(std::cout, std::cin, review_controller);
      _current_screen = ReturnInput::Screen::REVIEW_MENU;
  } else {
    throw NotImplementedError("GUI not implemented yet");
  }
}

void Driver::displayReviewGameScreen(std::string gameId){
    if (_display_type == CONSOLE) {
        std::shared_ptr<LocalBoardReview> board = std::make_shared<LocalBoardReview>(gameId);
        std::shared_ptr<ReviewGameController> rgame_controller = std::make_shared<ReviewGameController>(getClient(), board);
        _display = std::make_shared<ReviewGameConsole>(std::cout, std::cin, board, rgame_controller);
        _current_screen = ReturnInput::Screen::REVIEW_GAME;
    } else {
        throw NotImplementedError("GUI not implemented yet");
    }
}