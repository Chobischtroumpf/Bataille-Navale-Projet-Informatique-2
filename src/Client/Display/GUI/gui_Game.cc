/**
 * @file gui_Game.cc
 * @brief Implementation of the GUI Game class
 
 * TODO: Implementation for the normal mode and the commander mode
         Playing the game
         Waiting for the game
         Waiting for the turn
*/


#include "gui_Game.hh"
#include <QHBoxLayout>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <string>

BoardFrame::BoardFrame(Game *parent, std::shared_ptr<LocalBoardCommander> board, bool my_side) : _parent{parent}, _board(board), _my_side(my_side) {
  setMouseTracking(true);
}

QBrush BoardFrame::getTileColor(CellType cell) {
  QBrush brush;
  switch (cell) {
  case CellType::UNDAMAGED_SHIP:
    brush = Qt::gray;
    break;
  case CellType::HIT_SHIP:
    brush = Qt::red;
    break;
  case CellType::WATER:
    brush = Qt::blue;
    break;
  default:
    brush = Qt::white;
    break;
  }
  return brush;
}

void BoardFrame::drawCell(QPainter &painter, int x, int y, CellType cell) {
  QBrush brush = getTileColor(cell);
  painter.fillRect(x * 50, y * 50, 50, 50, brush);
  painter.setPen(Qt::white);
  painter.drawRect(x * 50, y * 50, 50, 50);
}

void BoardFrame::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event); // To avoid unused parameter warning
  QPainter painter(this);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (_my_side && _parent->getPhase() == PLACING_SHIPS && _parent->isShipSelected()){
        Ship ship = _parent->getSelectedShip().value();
        ship.setTopLeft(_last_hovered);
        if (ship.isAt(BoardCoordinates(i, j))) {
          drawCell(painter, i, j, CellType::UNDAMAGED_SHIP);
        } else {
          drawCell(painter, i, j, _board->cellType(_my_side, BoardCoordinates(i, j)));
        }
      } else {
        drawCell(painter, i, j, _board->cellType(_my_side, BoardCoordinates(i, j)));
      }
    }
  }
}

void BoardFrame::mousePressEvent(QMouseEvent *event) {
  if (_parent->getPhase() == PLACING_SHIPS && _parent->isShipSelected() && event->button() == Qt::LeftButton) {
    Ship ship = _parent->getSelectedShip().value();
    ship.setTopLeft(_last_hovered);
    _parent->placeShip(ship);
  } else if (event->button() == Qt::RightButton) {
    _parent->rotateShip();
  }

  _parent->refreshButtons();
  update();
}

void BoardFrame::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    _parent->rotateShip();
    update();
  }
  else if (event->key() == Qt::Key_Return) {
    if (_parent->getPhase() == PLACING_SHIPS && _parent->isShipSelected()) {
      Ship ship = _parent->getSelectedShip().value();
      ship.setTopLeft(_last_hovered);
      _parent->placeShip(ship);
    }
    _parent->refreshButtons();
    update();
  }
  else if (event->key() == 'Z') {
    _parent->previousShip();
    update();
  }
  else if (event->key() == 'X') {
    _parent->nextShip();
    update();
  }
}

void BoardFrame::mouseMoveEvent(QMouseEvent *event) {
  _last_hovered = BoardCoordinates(event->position().x() / 50, event->position().y() / 50);
  setFocus();
  update();
}

void Game::setupShipPlacement() {
  _phase = PLACING_SHIPS;
  _ships_placement_layout = new QHBoxLayout();
  QVBoxLayout *sizes = new QVBoxLayout();
  for (auto &ship : _board->shipsToPlace()) {
    std::string ship_name;
    switch (ship.first) {
      case 2:
        ship_name = "2. Destroyer (x" + std::to_string(ship.second) + ")";
        break;
      case 3:
        ship_name = "3. Cruiser (x" + std::to_string(ship.second) + ")";
        break;
      case 4:
        ship_name = "4. Battleship (x" + std::to_string(ship.second) + ")";
        break;
      case 5:
        ship_name = "5. Carrier (x" + std::to_string(ship.second) + ")";
        break;
    }
    QPushButton *button = new QPushButton(QString::fromStdString(ship_name));
    connect(button, &QPushButton::clicked, [this, ship] {
      _possible_ships = std::make_shared<ShipCommander>(ship.first);
      _selected_ship = _possible_ships->getShip();
    });
    _ships_buttons.push_back(button);
    sizes->addWidget(button);
  }
  QHBoxLayout *change_ships = new QHBoxLayout();
  QPushButton *change_ship = new QPushButton("Next >> (X)");
  QPushButton *change_ship_back = new QPushButton("(Z) << Previous");
  connect(change_ship, &QPushButton::clicked, this, &Game::nextShip);
  connect(change_ship_back, &QPushButton::clicked, this, &Game::previousShip);
  change_ships->addWidget(change_ship_back);
  change_ships->addWidget(change_ship);
  _ships_placement_layout->addLayout(sizes);
  _ships_placement_layout->addLayout(change_ships);

  _boards_layout = new QHBoxLayout();
  _boards_layout->addWidget(_my_frame);
  _boards_layout->addWidget(_their_frame);
  QVBoxLayout *main_layout = new QVBoxLayout();
  setLayout(main_layout);
  main_layout->addLayout(_boards_layout);
  main_layout->addLayout(_ships_placement_layout);
}

void Game::setupWaitingGame() {
  _phase = WAITING_GAME;
  // TODO: Implement
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

  nlohmann::json req;
  req["moveType"] = "StartGame";
  _game_client->MakeMove(session_id_str, req);

  sleep(1);
  
  _board= std::make_shared<LocalBoardCommander>(
      _game_client, Player(), GameMode::COMMANDER, session_id_str);
  _game_controller = std::make_shared<GameController>(_board);

  setWindowTitle("Game");

  resize(1200, 800);


  _my_frame = new BoardFrame(this, _board, true);

  _their_frame = new BoardFrame(this, _board, false);

  _my_frame->setFixedSize(500, 500);
  _their_frame->setFixedSize(500, 500);

  setupShipPlacement();
}

void Game::refreshButtons() {
  for (auto &button : _ships_buttons) {
    int size = std::stoi(button->text().toStdString().substr(0, 1));
    int count = _board->shipsToPlace().at(size);
    if (count == 0) {
      button->setEnabled(false);
    } else {
      std::string new_label;
      switch (size) {
        case 2:
          new_label = "2. Destroyer (x" + std::to_string(count) + ")";
          break;
        case 3:
          new_label = "3. Cruiser (x" + std::to_string(count) + ")";
          break;
        case 4:
          new_label = "4. Battleship (x" + std::to_string(count) + ")";
          break;
        case 5:
          new_label = "5. Carrier (x" + std::to_string(count) + ")";
          break;
      }
      button->setText(QString::fromStdString(new_label));
    }
  }
}

void Game::placeShip(Ship& ship) {
  _game_controller->placeShip(ship);
  _selected_ship = std::nullopt;
  _possible_ships = nullptr;
  if (_board->allShipsPlaced()) {
    setupWaitingGame();
  }
}

void Game::nextShip() {
  if (_possible_ships)
    _possible_ships->next();
    _selected_ship = _possible_ships->getShip();
}

void Game::previousShip() {
  if (_possible_ships)
    _possible_ships->previous();
    _selected_ship = _possible_ships->getShip();
}

bool Game::isShipSelected() const {
  return _selected_ship.has_value();
}

std::optional<Ship> Game::getSelectedShip() const {
  return _selected_ship;
}

void Game::rotateShip() {
  if (_selected_ship.has_value()) {
    _selected_ship->rotate();
  }
}

std::shared_ptr<ShipClassic> Game::getPossibleShips() const {
  return _possible_ships;
}

Phase Game::getPhase() const {
  return _phase;
}