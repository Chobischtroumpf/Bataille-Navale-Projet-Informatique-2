#pragma once

#include <cstddef>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>
#include <thread>

#include "board_coordinates.hh"
#include "cell.hh"
#include "game_client.hh"
#include "game_view.hh"
#include "player.hh"
#include "ship.hh"

typedef enum { CLASSIC, COMMANDER } GameMode;

/*
 * Local board view
 */
class LocalBoardCommander : public GameView {
private:
  Player _player;
  GameMode _mode;
  const std::string _session_id;

  std::vector<std::vector<Cell>> _my_board;
  std::vector<std::vector<Cell>> _their_board;

  std::string _my_username;
  std::string _their_username;

  std::shared_ptr<GameClient> _client;

  uint16_t _game_time = 0;
  uint16_t _player_time = 0;
  uint16_t _opponent_time = 0;
  bool _is_pendulum = false;

  // bool _my_turn;
  bool _is_finished;
  bool _is_victory;

  /* Get the cell in one of the board*/
  Cell get(bool my_side, BoardCoordinates position) const;

public:
  LocalBoardCommander(std::shared_ptr<GameClient> client, Player player,
                      GameMode mode, const std::string &sessionId);

  virtual ~LocalBoardCommander() override = default;

  // Getters
  bool myTurn() const override;
  bool isFinished() const override;
  bool isVictory() const override;
  std::size_t width() const override;
  std::size_t height() const override;
  GameMode mode() const;
  Player player() const;


  void setPlayerFaction(Faction faction);

  /*
   * Get the cell type at the given coordinates
   * @param coordinates : coordinates of the cell
   * @return le cell type
   */
  CellType cellType(bool my_side, BoardCoordinates coordinates) const override;

  std::shared_ptr<Ship> shipId(bool my_side, BoardCoordinates position) const;

  /* Check if two cells are part of the same ship */
  bool isSameShip(bool my_side, BoardCoordinates first,
                  BoardCoordinates second) const override;

  /* Get the neighbors of a cell */
  std::vector<Cell> getNeighbors(BoardCoordinates coord) const;

  /* Returns the ships that still need to be placed */
  PossibleShips shipsToPlace() const;

  /* Returns true if all boats are placed */
  bool allShipsPlaced() const;

  /* Returns true if the ship is still available */
  bool isShipAvailable(int size) const;

  /* Returns the ships that have been placed */
  std::vector<std::shared_ptr<Ship>> getPlacedShips() const;

  /* places a ship */
  void placeShip(Ship ship);

  /* Polls the server to wait the beggining of the game */
  bool waitGame();

  bool isGameStarted();

  /* Polls the server to wait the turn */
  void waitTurn();

  /* Asks once if its my turn*/
  bool fetchMyTurn();

  // returns the
  CellType best(CellType lhs, CellType rhs);

  static CellType string_to_celltype(const std::string &type);

  void updateBoard();

  bool isInBoard(BoardCoordinates coord) const;

  // sends the fire request to the server
  void fire(SpecialAbility ability, BoardCoordinates coordinates);

  std::string getMyUsername() const;
  std::string getTheirUsername() const;
  uint16_t getGameTime() const;
  uint16_t getPlayerTime() const;
  uint16_t getOpponentTime() const;
  bool isPendulum() const;
};