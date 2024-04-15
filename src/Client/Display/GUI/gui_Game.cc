#include "gui_Game.hh"
#include <QHBoxLayout>
#include <QString>
#include <QLabel>

BoardFrame::BoardFrame(QWidget *parent, LocalBoardCommander& board, bool my_side) : QFrame(parent), _board(board), _my_side(my_side) {
  setFrameStyle(QFrame::Panel | QFrame::Sunken);
  setFocusPolicy(Qt::StrongFocus);
  setGeometry(0, 0, 500, 500);
}

QBrush BoardFrame::getTileColor(CellType cell) {
  QBrush brush;
  switch (cell) {
  case CellType::WATER:
    brush = Qt::blue;
    break;
  case CellType::UNDAMAGED_SHIP:
    brush = Qt::gray;
    break;
  case CellType::HIT_SHIP:
    brush = Qt::red;
    break;
  }
  return brush;
}

void BoardFrame::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event); // To avoid unused parameter warning
  QPainter painter(this);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      QBrush brush = getTileColor(_board.cellType(_my_side, BoardCoordinates(i, j)));
      painter.fillRect(i * 50, j * 50, 50, 50, brush);
      painter.setPen(Qt::white);
      painter.drawRect(i * 50, j * 50, 50, 50);
    }
  }
}

void BoardFrame::mousePressEvent(QMouseEvent *event) {
  update();
}

Game::Game(std::shared_ptr<GameClient> gameClient) : _game_client(gameClient) {
  _game_client->Login("romek", "Romainlisa");
  sleep(1);

  njson gameDetails = {
      {"name", "oui"},
      {"gamemode", "Commander"},
      {"gameTimeLimit", 70},
      {"playerTimeLimit", 70},
      {"turnTimeLimit", 70},
      {"maxPlayers", 2}};

  GameClient second_client("http://localhost:8080");
  second_client.Login("STC", "Sashat");

  std::future<std::string> session_id = _game_client->CreateGame(gameDetails);

  std::string session_id_str = session_id.get();
  
  sleep(1);

  second_client.JoinGame(session_id_str);
  
  _board= std::make_shared<LocalBoardCommander>(
      _game_client, Player(), GameMode::COMMANDER, session_id_str);
  _game_controller = std::make_shared<GameController>(_board);

  setWindowTitle("Game");

  resize(1200, 800);


  _my_frame = new BoardFrame(this, *_board, true);

  _their_frame = new BoardFrame(this, *_board, false);
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(_my_frame);
  layout->addWidget(_their_frame);

  setLayout(layout);
}