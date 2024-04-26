/**
 * @file gui_Game.cc
 * @brief Implementation of the GUI Game class
 
 * TODO: Change Turn
*/


#include "gui_Game.hh"
#include "faction_bombardement.hh"
#include "faction_classique.hh"
#include "faction_mines.hh"
#include "faction_sonar.hh"
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
  std::vector<BoardCoordinates> coords;
  if (_parent->isAbilitySelected()) {
    if (_parent->getSelectedAbility()->getType() == AERIAL_STRIKE) {
      for (auto it = _parent->beginAerialStrike(_last_hovered); it != _parent->endAerialStrike(_last_hovered); ++it) {
        coords.push_back(*it);
      }
    } else if (_parent->getSelectedAbility()->getType() == PIERCING_TORPEDO) {
      for (auto it = _parent->beginPiercingTorpedo(_last_hovered); it != _parent->endPiercingTorpedo(_last_hovered); ++it) {
        coords.push_back(*it);
      }
    } else if (_parent->getSelectedAbility()->getType() == BIG_TORPEDO) {
      for (auto it = _parent->beginBigTorpedo(_last_hovered); it != _parent->endBigTorpedo(_last_hovered); ++it) {
        coords.push_back(*it);
      }
    } else {
      coords.push_back(_last_hovered);
    }
  }
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
      else if (!_my_side && _parent->getPhase() == PLAYING && _parent->isAbilitySelected()) {
        bool painted = false;
        for (auto &coord : coords) {
          if (coord == BoardCoordinates(i, j)) {
            painted = true;
            drawCell(painter, i, j, CellType::HIT_SHIP);
            break;
          }
        }
        if (!painted) {
          drawCell(painter, i, j, _board->cellType(_my_side, BoardCoordinates(i, j)));
        }
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

  // Putting times over the boards
  QVBoxLayout *my_layout = new QVBoxLayout();
  _my_label = new QLabel(QString::fromStdString(_board->getMyUsername() + "'s board - " + std::to_string(_board->getPlayerTime()) + "s"));
  my_layout->addWidget(_my_label);
  my_layout->addWidget(_my_frame);

  QVBoxLayout *their_layout = new QVBoxLayout();
  _their_label = new QLabel(QString::fromStdString(_board->getTheirUsername() + "'s board - " + std::to_string(_board->getOpponentTime()) + "s"));
  their_layout->addWidget(_their_label);
  their_layout->addWidget(_their_frame);

  _game_label = new QLabel("Game time: " + QString::number(_board->getGameTime()) + "s");
  _game_label->setAlignment(Qt::AlignCenter);

  _boards_layout = new QHBoxLayout();
  _boards_layout->addLayout(my_layout);
  _boards_layout->addLayout(their_layout);
  QVBoxLayout *main_layout = new QVBoxLayout();
  setLayout(main_layout);
  main_layout->addWidget(_game_label);
  main_layout->addLayout(_boards_layout);
  main_layout->addLayout(_footer_layout);
}

void Game::setupSpectating() {
  _phase = SPECTATING;

  _footer_layout = new QHBoxLayout();
  QLabel *phase2Label = new QLabel(QString::fromStdString("Spectating " + _board->getMyUsername() + " vs " + _board->getTheirUsername()));

  // Add widgets to layout
  _footer_layout->addWidget(phase2Label);
  // Putting times over the boards
  QVBoxLayout *my_layout = new QVBoxLayout();
  _my_label = new QLabel(QString::fromStdString(_board->getMyUsername() + "'s board - " + std::to_string(_board->getPlayerTime()) + "s"));
  my_layout->addWidget(_my_label);
  my_layout->addWidget(_my_frame);

  QVBoxLayout *their_layout = new QVBoxLayout();
  _their_label = new QLabel(QString::fromStdString(_board->getTheirUsername() + "'s board - " + std::to_string(_board->getOpponentTime()) + "s"));
  their_layout->addWidget(_their_label);
  their_layout->addWidget(_their_frame);

  _game_label = new QLabel("Game time: " + QString::number(_board->getGameTime()) + "s");
  _game_label->setAlignment(Qt::AlignCenter);

  _boards_layout = new QHBoxLayout();
  _boards_layout->addLayout(my_layout);
  _boards_layout->addLayout(their_layout);
  QVBoxLayout *main_layout = new QVBoxLayout();
  setLayout(main_layout);
  main_layout->addWidget(_game_label);
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
  energy = _board->player().getEnergyPoints();

  if (_abilities_buttons.size() > 0) {
    _abilities_buttons.clear();
    _ability_informations.clear();
  }

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

void Game::setupWinning() {
  _phase = FINISHED;
  clearLayout(_footer_layout);

  QLabel *phase2Label = new QLabel("You won! 🎶 You're simply the best 🎶");

  QPushButton *quitButton = new QPushButton("Return to main menu");

  connect(quitButton, &QPushButton::clicked, [this] {
    emit gameFinished();
    this->close();
  });

  phase2Label->setAlignment(Qt::AlignCenter);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(phase2Label);
  layout->addWidget(quitButton);

  _footer_layout->addLayout(layout);
}

void Game::setupLosing() {
  _phase = FINISHED;
  clearLayout(_footer_layout);

  QLabel *phase2Label = new QLabel("You lost! 🫵😹 Might be bad luck, or just a skill issue...");

  QPushButton *quitButton = new QPushButton("Return to main menu");

  connect(quitButton, &QPushButton::clicked, [this] {
    emit gameFinished();
    this->close();
  });

  phase2Label->setAlignment(Qt::AlignCenter);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(phase2Label);
  layout->addWidget(quitButton);

  _footer_layout->addLayout(layout);
}

void Game::setupFinished() {
  _phase = FINISHED;
  clearLayout(_footer_layout);

  QLabel *phase2Label = new QLabel("The game is finished");

  QPushButton *quitButton = new QPushButton("Return to main menu");

  connect(quitButton, &QPushButton::clicked, [this] {
    emit gameFinished();
    this->close();
  });

  phase2Label->setAlignment(Qt::AlignCenter);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(phase2Label);
  layout->addWidget(quitButton);

  _footer_layout->addLayout(layout);
}

Game::Game(std::shared_ptr<GameClient> gameClient, std::string session_id,
           int selected_faction, bool commander_mode, bool spectating)
    : _game_client(gameClient), _session_id{session_id},
      _commander_mode(commander_mode), _spectating(spectating) {
  Player player = Player();
  Faction faction;
  if (commander_mode) {
    switch (selected_faction) {
      case 0:
        player.setFaction(FactionBombardement());
        break;
      case 1:
        player.setFaction(FactionSonar());
        break;
      case 2:
        player.setFaction(FactionMines());
        break;
    }
  } else {
    player.setFaction(FactionClassique());
  }

  _board= std::make_shared<LocalBoardCommander>(
      _game_client, player, (commander_mode ? GameMode::COMMANDER : GameMode::CLASSIC), _session_id);
  _game_controller = std::make_shared<GameController>(_board);

  setWindowTitle(QString::fromStdString("Battleship: " + _board->getMyUsername() + " vs " + _board->getTheirUsername()));

  resize(1200, 800);

  _timer = new QTimer(this);
  connect(_timer, &QTimer::timeout, this, &Game::update);
  _timer->start(1000);

  _my_frame = new BoardFrame(this, _board, true);

  _their_frame = new BoardFrame(this, _board, false);

  _my_frame->setFixedSize(500, 500);
  _their_frame->setFixedSize(500, 500);

  if (_spectating) {
    setupSpectating();
  } else {
    setupShipPlacement();
  }
}

Game::~Game() {
  delete _timer;
  delete _my_frame;
  delete _their_frame;
  delete _game_label;
  delete _my_label;
  delete _their_label;
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
  } else if (_phase == SPECTATING) {
    _board->updateBoard();
    if (_board->isFinished()) {
      setupFinished();
    }
  }

  // Update Timers
  if (_phase == PLAYING || _phase == WAITING_TURN) {
    _board->updateBoard();
    updateTimerLabels();
  }
}

void Game::updateTimerLabels() {
  _game_label->setText("Game time: " + QString::number(_board->getGameTime()) + "s");
  _my_label->setText(QString::fromStdString(_board->getMyUsername() + "'s board - " + std::to_string(_board->getPlayerTime()) + "s"));
  _their_label->setText(QString::fromStdString(_board->getTheirUsername() + "'s board - " + std::to_string(_board->getOpponentTime()) + "s"));
}

void Game::updateAbilityInformations() {
  energy = _board->player().getEnergyPoints();
  if (_selected_ability) {
    for (auto &button : _abilities_buttons) {
      if (button->text().toStdString() == _selected_ability->getName()) {
        button->setStyleSheet("font-weight: bold;");
      } else {
        button->setStyleSheet("");
      }
    }
  }
  if (_selected_ability == nullptr) {
    _ability_informations.at(0)->setText("Selected ability: ");
    _ability_informations.at(1)->setText("Description: ");
    _ability_informations.at(2)->setText("Energy cost: ");
  } else {
    _ability_informations.at(0)->setText(
        "Selected ability: " +
        QString::fromStdString(_selected_ability->getName()));
    _ability_informations.at(1)->setText(
        "Description: " +
        QString::fromStdString(_selected_ability->getDescription()));
    _ability_informations.at(2)->setText(
        "Energy cost: " + QString::number(_selected_ability->getEnergyCost()));
  }
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
  if (_possible_ships) {
    _possible_ships->next();
    _selected_ship = _possible_ships->getShip();
  }
}

void Game::previousShip() {
  if (_possible_ships) {
    _possible_ships->previous();
    _selected_ship = _possible_ships->getShip();
  }
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
      _selected_ability = nullptr;
      updateAbilityInformations();
      if (_board->isFinished()) {
        if (_board->isVictory()) {
          setupWinning();
        } else {
          setupLosing();
        }
      } else {
        setupWaitingTurn();
      }
    }
  }
}

const SpecialAbility* Game::getSelectedAbility() {
  return _selected_ability;
}

AerialStrikeIterator Game::beginAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords);
}

AerialStrikeIterator Game::endAerialStrike(BoardCoordinates coords) {
  return AerialStrikeIterator(coords + BoardCoordinates(4,3));
}

PiercingTorpedoIterator Game::beginPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords);
}

PiercingTorpedoIterator Game::endPiercingTorpedo(BoardCoordinates coords) {
  return PiercingTorpedoIterator(coords + BoardCoordinates(0,4));
}

BigTorpedoIterator Game::beginBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords);
}

BigTorpedoIterator Game::endBigTorpedo(BoardCoordinates coords) {
  return BigTorpedoIterator(coords + BoardCoordinates(2,0));
}