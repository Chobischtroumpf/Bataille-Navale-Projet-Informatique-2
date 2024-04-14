#pragma once

#include <memory>
#include <QWidget>
#include <QPainter>
#include <QFrame>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "gui.hh"
#include "board_coordinates.hh"
#include "game_view.hh"
#include "game_controller.hh"
#include "game_client.hh"
#include "local_board_commander.hh"

class BoardFrame : public QFrame {
  Q_OBJECT

public:
  BoardFrame(QWidget *parent, LocalBoardCommander& board, bool my_side = true);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  LocalBoardCommander _board;
  bool _my_side;
};

class Game : public QWidget {
    Q_OBJECT

public:
    Game(std::shared_ptr<GameClient> gameClient);
    void display();
    void display_error();
    void update();
    void handle_input(BoardCoordinates user_input);

protected:

private:
    std::shared_ptr<GameClient> _game_client;
    std::shared_ptr<LocalBoardCommander> _board;
    std::shared_ptr<GameController> _game_controller;

    BoardFrame *_my_frame;
    BoardFrame *_their_frame;
};