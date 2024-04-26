#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>
#include <QPushButton>
#include <QThread>
#include <QLabel>
#include <vector>
#include <optional>

#include "board_coordinates.hh"
#include "game_client.hh"
#include "game_controller.hh"
#include "game_view.hh"
#include "gui.hh"
#include "local_board_commander.hh"
#include "ship_classic.hh"
#include "ship_commander.hh"
#include "piercing_torpedo_iterator.hh"
#include "big_torpedo_iterator.hh"
#include "aerial_strike_iterator.hh"

enum Phase { PLACING_SHIPS, WAITING_GAME, PLAYING, WAITING_TURN, FINISHED, SPECTATING };

class Game;

/**
 * @brief Frame representing a board, used to display the game
 */
class BoardFrame : public QFrame {
  Q_OBJECT

public:
  /**
   * @brief Construct a new Board Frame object
   *
   * @param parent The parent game
   * @param board The board to display
   * @param my_side True if the board is the player's board, false otherwise
   */
  BoardFrame(Game *parent, std::shared_ptr<LocalBoardCommander> board, bool my_side = true);

  /**
   * @brief Get the color of a tile
   *
   * @param cell The cell to get the color of
   * @return QBrush The color of the tile
   */
  QBrush getTileColor(CellType cell);

protected:

  /**
   * @brief Draw a cell on the board
   *
   * @param painter The painter to use
   * @param x The x coordinate of the cell
   * @param y The y coordinate of the cell
   * @param cell The cell to draw
   * @return void
   */
  void drawCell(QPainter &painter, int x, int y, CellType cell);

  /**
   * @brief Draw the board
   *
   * @param event The paint event
   * @return void
   */
  void paintEvent(QPaintEvent *event) override;

  /**
   * @brief Handle mouse press event
   *
   * @param event The mouse event
   * @return void
   */
  void mousePressEvent(QMouseEvent *event) override;

  /**
   * @brief Handle mouse move event (called when the mouse is moved on the board)
   *
   * @param event The mouse event
   * @return void
   */
  void mouseMoveEvent(QMouseEvent *event) override;

  /**
   * @brief Handle key press event
   *
   * @param event The key event
   * @return void
   */
  void keyPressEvent(QKeyEvent *event) override;

private:
  std::shared_ptr<LocalBoardCommander> _board; //< The board to display
  bool _my_side; //< True if the board is the player's board, false if the other player's board
  BoardCoordinates _last_hovered = BoardCoordinates(0, 0); //< The last cell hovered by the mouse
  Game *_parent; //< The parent game
};


/**
 * @brief The main game class, used to display the game window
 */
class Game : public QWidget {
  Q_OBJECT

public:
  Game(std::shared_ptr<GameClient> gameClient, std::string session_id, int selected_faction, bool commander_mode = true, bool spectating = false);
  ~Game();
  void display();
  void display_error();
  void update();
  void handle_input(BoardCoordinates user_input);

  /**
   * @brief Getters of different attributes of the window
   * @return The attribute
  */
  bool isShipSelected() const;
  bool isAbilitySelected() const;
  std::optional<Ship> getSelectedShip() const;
  std::shared_ptr<ShipClassic> getPossibleShips() const;
  Phase getPhase() const;
  const SpecialAbility* getSelectedAbility();

  /**
   * @brief Place a ship on the board
   * @param ship The ship to place
   */
  void placeShip(Ship& ship);

  /**
   * @brief Refresh the ship placement buttons
   */
  void refreshButtons();

  /**
   * @brief Rotate the selected ship
   */
  void rotateShip();

  /**
   * @brief Go to the previous ship of the ShipClassic or ShipCommander
   */
  void previousShip();

  /**
   * @brief Go to the next ship of the ShipClassic or ShipCommander
   */
  void nextShip();

  /**
   * @brief Clear a given layout
   * @param layout The layout to clear
   */
  void clearLayout(QLayout *layout);

  /**
   * @brief Update the ability informations
   */
  void updateAbilityInformations();

  /**
   * @brief Update the timer labels
   */
  void updateTimerLabels();

  /**
   * @brief fire a shot at the given coordinates
   */
  void fire(BoardCoordinates coord);

  // Iterators for special abilities
  AerialStrikeIterator beginAerialStrike(BoardCoordinates coords);
  AerialStrikeIterator endAerialStrike(BoardCoordinates coords);

  PiercingTorpedoIterator beginPiercingTorpedo(BoardCoordinates coords);
  PiercingTorpedoIterator endPiercingTorpedo(BoardCoordinates coords);

  BigTorpedoIterator beginBigTorpedo(BoardCoordinates coords);
  BigTorpedoIterator endBigTorpedo(BoardCoordinates coords);

protected:
  // Setup the layout of the window
  void setupShipPlacement();
  void setupSpectating();
  void setupGame();
  void setupWaitingGame();
  void setupWaitingTurn();
  void setupWinning();
  void setupLosing();
  void setupFinished();

signals:
  // Signal emitted when the game is finished
  void gameFinished();

private:
  std::shared_ptr<GameClient> _game_client; //< The game client
  std::shared_ptr<LocalBoardCommander> _board; //< The local board of the game
  std::shared_ptr<GameController> _game_controller; //< The game controller

  BoardFrame *_my_frame; //< The frame representing the player's board
  BoardFrame *_their_frame; //< The frame representing the other player's board
  QHBoxLayout *_boards_layout; //< The layout containing the boards
  QHBoxLayout *_footer_layout; //< The layout containing the footer
  QLabel *_my_label; //< Timer label for the player
  QLabel *_their_label; //< Timer label for the other player
  QLabel *_game_label; //< Label for the game timer
  std::vector<QPushButton *> _ShipsButtons; //< The buttons to select the ships
  std::string _session_id; //< The session id of the game
  std::optional<Ship> _selected_ship = std::nullopt; //< The selected ship
  std::shared_ptr<ShipClassic> _possible_ships = nullptr; //< The ships that can be placed
  Phase _phase; //< The current phase of the game
  std::vector<QPushButton *> _ships_buttons; //< The buttons to select the ships
  bool _commander_mode; //< True if the player is the commander, false otherwise
  bool _spectating; //< True if the player is spectating, false otherwise
  QTimer *_timer; //< The timer used to update the game
  const SpecialAbility* _selected_ability = nullptr; //< The selected ability
  int energy = 0; //< The energy of the player
  std::vector<QPushButton *> _abilities_buttons; //< The buttons to select the abilities
  std::vector<QLabel *> _ability_informations; //< The labels to display the ability informations
  std::vector<QLabel *> _game_informations; //< The labels to display the game informations (energy)
};