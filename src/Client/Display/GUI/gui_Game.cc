/**
 * @file gui_Game.cc
 * @brief Implementation of the GUI Game class
 
 * TODO: Change Turn
*/


#include "gui_Game.hh"
#include "faction_bombardement.hh"
#include <QHBoxLayout>
#include <QString>
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
  case CellType::OCEAN:
    brush = Qt::cyan;
    break;
  case CellType::SCANNED_SHIP:
    brush = Qt::yellow;
    break;
  case CellType::SUNK_SHIP:
    brush = Qt::darkRed;
    break;
  case CellType::HIT_MINE:
    brush = Qt::black;
    break;
  case CellType::SCANNED_MINE:
    brush = Qt::darkYellow;
    break;
  case CellType::WATER:
    brush = Qt::blue;
    break;
  case CellType::UNDAMAGED_MINE:
    brush = Qt::black;
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
      } 
      else if (!_my_side && _parent->getPhase() == PLAYING && _parent->isAbilitySelected() && _last_hovered == BoardCoordinates(i, j)) {
        drawCell(painter, i, j, CellType::HIT_SHIP);
      }
      else {
        drawCell(painter, i, j, _board->cellType(_my_side, BoardCoordinates(i, j)));
      }
    }
  }
}

void BoardFrame::mousePressEvent(QMouseEvent *event) {
  if (_my_side && _parent->getPhase() == PLACING_SHIPS && _parent->isShipSelected() && event->button() == Qt::LeftButton) {
    Ship ship = _parent->getSelectedShip().value();
    ship.setTopLeft(_last_hovered);
    _parent->placeShip(ship);
    _parent->refreshButtons();
  } else if (_my_side && event->button() == Qt::RightButton) {
    _parent->rotateShip();
  } else if (!_my_side && _parent->getPhase() == PLAYING && _parent->isAbilitySelected()) {
    _parent->fire(_last_hovered);
  }

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

void Game::clearLayout(QLayout* layout) {
  QLayoutItem *item;
  if (layout == nullptr)
    return;
  while ((item = layout->takeAt(0)) != nullptr) {
    if (item->layout()) {
      item->layout()->setParent(nullptr);
      clearLayout(item->layout());
      item->layout()->deleteLater();
    }
    if (item->widget()) {
      item->widget()->setParent(nullptr);
      item->widget()->hide();
      item->widget()->deleteLater();
    }
    delete item;
  }
}

void Game::setupShipPlacement() {
  _phase = PLACING_SHIPS;
  _footer_layout = new QHBoxLayout();
  QVBoxLayout *sizes = new QVBoxLayout();
  for (auto &ship : _board->shipsToPlace()) {
    std::string ship_name;
    switch (ship.first) {
      case 1:
        ship_name = "1. Bomb (x" + std::to_string(ship.second) + ")";
        break;
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
      if (_commander_mode)
        _possible_ships = std::make_shared<ShipCommander>(ship.first);
      else
        _possible_ships = std::make_shared<ShipClassic>(ship.first);
      _selected_ship = _possible_ships->getShip();
    });
    _ships_buttons.push_back(button);
    sizes->addWidget(button);
  }
  QHBoxLayout *change_ships = new QHBoxLayout();
  QVBoxLayout *controls = new QVBoxLayout();
  QPushButton *change_ship = new QPushButton("Next >> (X)");
  QPushButton *change_ship_back = new QPushButton("(Z) << Previous");
  QPushButton *rotate_ship = new QPushButton("Rotate (Space or R-Click)");
  QLabel *place_ship = new QLabel("Place ship (Enter or L-Click)");
  place_ship->setAlignment(Qt::AlignCenter);
  place_ship->setStyleSheet("font-weight: bold;");
  place_ship->setFixedHeight(30);
  connect(change_ship, &QPushButton::clicked, this, &Game::nextShip);
  connect(change_ship_back, &QPushButton::clicked, this, &Game::previousShip);
  connect(rotate_ship, &QPushButton::clicked, this, &Game::rotateShip);
  change_ships->addWidget(change_ship_back);
  change_ships->addWidget(change_ship);
  controls->addWidget(rotate_ship);
  controls->addLayout(change_ships);
  controls->addWidget(place_ship);
  _footer_layout->addLayout(sizes);
  _footer_layout->addLayout(controls);

  _boards_layout = new QHBoxLayout();
  _boards_layout->addWidget(_my_frame);
  _boards_layout->addWidget(_their_frame);
  QVBoxLayout *main_layout = new QVBoxLayout();
  setLayout(main_layout);
  main_layout->addLayout(_boards_layout);
  main_layout->addLayout(_footer_layout);
}

void Game::setupWaitingGame() {
  _phase = WAITING_GAME;
  clearLayout(_footer_layout);

  QLabel *phase2Label = new QLabel("Waiting for the game to start");

  phase2Label->setAlignment(Qt::AlignCenter);

  // Add widgets to layout
  _footer_layout->addWidget(phase2Label);
}

void Game::setupWaitingTurn() {
  _phase = WAITING_TURN;
  clearLayout(_footer_layout);

  QLabel *phase2Label = new QLabel("Waiting for your turn");

  phase2Label->setAlignment(Qt::AlignCenter);

  // Add widgets to layout
  _footer_layout->addWidget(phase2Label);
}

void Game::setupGame() {
  _phase = PLAYING;
  clearLayout(_footer_layout);

  QVBoxLayout *game_layout = new QVBoxLayout();

  QHBoxLayout *ability_layout = new QHBoxLayout();
  for (auto &ability : _board->player().getFaction().getSpecialAbilities()) {
    QPushButton *button = new QPushButton(QString::fromStdString(ability.getName()));
    connect(button, &QPushButton::clicked, [this, ability] {
      _selected_ability = &ability;
      updateAbilityInformations();
    });
    if (ability.getEnergyCost() > energy) {
      button->setEnabled(false);
    }
    _abilities_buttons.push_back(button);
    ability_layout->addWidget(button);
  }
  game_layout->addLayout(ability_layout);

  QHBoxLayout *information_layout = new QHBoxLayout();
  QVBoxLayout *game_informations_layout = new QVBoxLayout();
  QVBoxLayout *selected_ability_information_layout = new QVBoxLayout();

  QLabel *energy_label = new QLabel("Energy: " + QString::number(energy));
  QLabel *selected_ability_name = new QLabel("Selected ability: ");
  QLabel *selected_ability_description = new QLabel("Description: ");
  QLabel *selected_ability_energy = new QLabel("Energy cost: ");

  _ability_informations.push_back(selected_ability_name);
  _ability_informations.push_back(selected_ability_description);
  _ability_informations.push_back(selected_ability_energy);

  _game_informations.push_back(energy_label);

  game_informations_layout->addWidget(energy_label);
  selected_ability_information_layout->addWidget(selected_ability_name);
  selected_ability_information_layout->addWidget(selected_ability_description);
  selected_ability_information_layout->addWidget(selected_ability_energy);

  information_layout->addLayout(game_informations_layout);
  information_layout->addLayout(selected_ability_information_layout);

  game_layout->addLayout(information_layout);

  _footer_layout->addLayout(game_layout); 
}

Game::Game(std::shared_ptr<GameClient> gameClient, bool commander_mode) : _game_client(gameClient), _commander_mode(commander_mode) {
  _game_client->Login("Jeffries", "Hitchcock");
  sleep(1);

  njson gameDetails = {
      {"name", "oui"},
      {"gamemode", "Commander"},
      {"gameTimeLimit", 70},
      {"playerTimeLimit", 70},
      {"turnTimeLimit", 70},
      {"maxPlayers", 2}};

  std::future<std::string> session_id = _game_client->CreateGame(gameDetails);

  std::string session_id_str = session_id.get();
  std::string ouifqsf;
  std::cin >> ouifqsf;
  nlohmann::json req;
  req["moveType"] = "StartGame";
  _game_client->MakeMove(session_id_str, req);

  _board= std::make_shared<LocalBoardCommander>(
      _game_client, Player(), GameMode::COMMANDER, session_id_str);
  _game_controller = std::make_shared<GameController>(_board);

  _board->setPlayerFaction(FactionBombardement());
  setWindowTitle("Game");

  resize(1200, 800);

  _timer = new QTimer(this);
  connect(_timer, &QTimer::timeout, this, &Game::update);
  _timer->start(1000);

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
    }
    std::string new_label;
    switch (size) {
    case 1:
      new_label = "1. Bomb (x" + std::to_string(count) + ")";
      break;
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

void Game::update() {
  if (_phase == WAITING_GAME) {
    if (_board->isGameStarted()) {
      if (_board->fetchMyTurn()) {
        _phase = PLAYING;
        setupGame();
      } else {
        _phase = WAITING_TURN;
        setupWaitingTurn();
      }
    }
  } else if (_phase == WAITING_TURN) {
    if (_board->fetchMyTurn()) {
      _board->updateBoard();
      setupGame();
    }
  }
}

void Game::updateAbilityInformations() {
  if (_selected_ability) {
    for (auto &button : _abilities_buttons) {
      if (button->text().toStdString() == _selected_ability->getName()) {
        button->setStyleSheet("font-weight: bold;");
      } else {
        button->setStyleSheet("");
      }
    }
  }
  _ability_informations.at(0)->setText("Selected ability: " + QString::fromStdString(_selected_ability->getName()));
  _ability_informations.at(1)->setText("Description: " + QString::fromStdString(_selected_ability->getDescription()));
  _ability_informations.at(2)->setText("Energy cost: " + QString::number(_selected_ability->getEnergyCost()));
}

void Game::placeShip(Ship& ship) {
  _game_controller->placeShip(ship);
  _selected_ship = std::nullopt;
  _possible_ships = nullptr;
  if (_board->allShipsPlaced()) {
    _phase = WAITING_GAME;
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

bool Game::isAbilitySelected() const {
  return _selected_ability != nullptr;
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

void Game::fire(BoardCoordinates coord) {
  if (_selected_ability) {
    if (_game_controller->fire(*_selected_ability, coord)) {
      energy -= _selected_ability->getEnergyCost();
      updateAbilityInformations();
      _selected_ability = nullptr;
      updateAbilityInformations();
      setupWaitingTurn();
    }
  }
}