#pragma once

#include <optional>
#include <stdexcept>

#include "ship.hh"
#include "cell_type.hh"

/*
 * Case dans le tableau
 */

class Cell {
  private:
    CellType _type{WATER};
    std::shared_ptr<Ship> _ship{nullptr};

  public:
    /** Default constructor: creates an unkown/Water cell */
    Cell() = default;
    Cell(CellType type, std::shared_ptr<Ship> ship)
        : _type{type}, _ship{ship} {
      if (type & UNDAMAGED_SHIP && !_ship) {
        throw std::logic_error("Cell with ship but no ship_id");
      } else if (!(type & UNDAMAGED_SHIP) && _ship) {
        throw std::logic_error("Cell without undamaged ship but with ship_id");
      }
    }

    //Getters
    std::shared_ptr<Ship> ship() const { return _ship; }
    constexpr inline CellType type() const { return _type; }

    //Setters
    void setShip(std::shared_ptr<Ship> ship) { _ship = ship; }
    void setType(CellType newType) { _type = newType; }
};
