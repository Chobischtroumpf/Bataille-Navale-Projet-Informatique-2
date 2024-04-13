#pragma once

#include <cstddef>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

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
  bool _is_finished;
  bool _is_victory;

  std::vector<std::vector<Cell>> _my_board;
  std::vector<std::vector<Cell>> _their_board;

  std::string _my_username;
  std::string _their_username;

  const std::string _session_id;
  std::shared_ptr<GameClient> _client;

  // bool _my_turn;

  /* Get the cell in one of the board*/
  Cell get(bool my_side, BoardCoordinates position) const;

public:
  LocalBoardCommander(std::shared_ptr<GameClient> client, Player player,
                      GameMode mode, const std::string &session_id);

  virtual ~LocalBoardCommander() override = default;

  // Getters
  bool myTurn() const override;
  bool isFinished() const override;
  bool isVictory() const override;
  std::size_t width() const override;
  std::size_t height() const override;
  GameMode mode() const;
  Player player() const;

  /*
   * Get the cell type at the given coordinates
   * @param coordinates : coordinates of the cell
   * @return le cell type
   */
  CellType cellType(bool my_side, BoardCoordinates coordinates) const override;

  std::optional<Ship> shipId(bool my_side, BoardCoordinates position) const;

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
  std::vector<Ship> getPlacedShips() const;

  /* places a ship */
  void placeShip(Ship ship);

  /* Polls the server to wait the beggining of the game */
  bool waitGame();

  /* Polls the server to wait the turn */
  void waitTurn();

  // void update() override { throw NotImplementedError("Update"); }

  // returns the
  CellType best(CellType lhs, CellType rhs);

  static CellType string_to_celltype(const std::string &type);

  void updateBoard(const nlohmann::json &new_board);

  bool isInBoard(BoardCoordinates coord) const;

  // sends the fire request to the server
  void fire(SpecialAbility ability, BoardCoordinates coordinates);

  std::string getMyUsername() const;
  std::string getTheirUsername() const;
};