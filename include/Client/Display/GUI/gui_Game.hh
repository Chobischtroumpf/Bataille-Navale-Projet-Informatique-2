#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>
#include <QPushButton>
#include <vector>
#include <optional>

#include "board_coordinates.hh"
#include "game_client.hh"
#include "game_controller.hh"
#include "game_view.hh"
#include "gui.hh"
#include "local_board_commander.hh"

enum Phase { PLACING_SHIPS, WAITING_GAME, PLAYING, WAITING_TURN, FINISHED };

class Game;

class BoardFrame : public QFrame {
  Q_OBJECT

public:
  BoardFrame(Game *parent, std::shared_ptr<LocalBoardCommander> board, bool my_side = true);
  QBrush getTileColor(CellType cell);

protected:
  void drawCell(QPainter &painter, int x, int y, CellType cell);
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  std::shared_ptr<LocalBoardCommander> _board;
  bool _my_side;
  BoardCoordinates _last_hovered = BoardCoordinates(0, 0);
  Game *_parent;
};


class Game : public QWidget {
  Q_OBJECT

public:
  Game(std::shared_ptr<GameClient> gameClient);
  void display();
  void display_error();
  void update();
  void handle_input(BoardCoordinates user_input);

  bool isShipSelected() const;
  std::optional<Ship> getSelectedShip() const;
  std::shared_ptr<ShipClassic> getPossibleShips() const;
  Phase getPhase() const;
  void placeShip(Ship& ship);
  void refreshButtons();
  void rotateShip();
  void previousShip();
  void nextShip();
  

protected:
  void setupShipPlacement();
  void setupGame();
  void setupWaitingGame();
  void setupWaitingTurn();
private:
  std::shared_ptr<GameClient> _game_client;
  std::shared_ptr<LocalBoardCommander> _board;
  std::shared_ptr<GameController> _game_controller;

  BoardFrame *_my_frame;
  BoardFrame *_their_frame;
  QHBoxLayout *_boards_layout;
  QHBoxLayout *_ships_placement_layout;
  std::vector<QPushButton *> _ShipsButtons;
  std::optional<Ship> _selected_ship = std::nullopt;
  std::shared_ptr<ShipCommander> _possible_ships = nullptr;                 //< The ships that can be placed
  Phase _phase = PLACING_SHIPS;
  std::vector<QPushButton *> _ships_buttons;
};