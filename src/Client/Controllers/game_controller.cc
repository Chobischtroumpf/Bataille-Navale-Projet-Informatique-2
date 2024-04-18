#include "game_controller.hh"
#include "board_coordinates.hh"
#include "cell_type.hh"
#include "local_board_commander.hh"
#include <iostream>

GameController::GameController(std::shared_ptr<LocalBoardCommander> board)
    : _board{std::move(board)} {}

bool GameController::fire(SpecialAbility ability,
                          BoardCoordinates coord) const {
  // Sends POST request to fire to the gameServer
  std::clog << "GameController::fire" << std::endl;
  if (!(_board->cellType(false, coord) & CellType::IS_HIT)) {
    std::clog << "GameController::fire: not IS_HIT" << std::endl;
    if (_board->mode() == GameMode::CLASSIC) {
      std::clog << "GameController::fire: CLASSIC" << std::endl;
      _board->fire(ability, coord);
    } else if (_board->mode() == GameMode::COMMANDER) {
      std::clog << "GameController::fire: COMMANDER" << std::endl;
      if (ability.getEnergyCost() == 0) {
        std::clog << "GameController::fire: COMMANDER: energyCost == 0" << std::endl;
        _board->fire(ability, coord);
        return true;
      }
      if (_board->player().getEnergyPoints() >= ability.getEnergyCost()) {
        std::clog << "GameController::fire: COMMANDER: energyCost <= energyPoints" << std::endl;
        _board->player().removeEnergyPoints(ability.getEnergyCost());
        _board->fire(ability, coord);
      }
    }
  }
  return false;
}

bool GameController::checkShipPosition(Ship ship) const {
  if (_board->isShipAvailable(ship.getLength()) == false) {
    return false;
  }
  for (auto coord : ship.getCoordinates()) {
    if (!_board->isInBoard(coord) ||
    _board->cellType(true,ship.getTopLeft() + coord) != CellType::WATER)
      return false;
    if (_board->mode() == GameMode::CLASSIC) {
      std::vector<Cell> cells = _board->getNeighbors(ship.getTopLeft() + coord);
      for (auto cell : cells) {
        if (cell.type() != CellType::WATER)
          return false;
      }
    }
  }
  return true;
}

bool GameController::placeShip(Ship ship) const {
  std::clog << "GameController::placeShip" << std::endl;
  // Verifier qu'on peut poser le bateau la
  if (checkShipPosition(ship)) {
    // Sends a request to place the ship to the gameServer
    _board->placeShip(ship);
    return true;
  } else {
    return false;
  }
}

void GameController::quit() {}
