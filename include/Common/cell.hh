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
    std::optional<Ship> _ship{std::nullopt};

  public:
    /** Default constructor: creates an unkown/Water cell */
    Cell() = default;
    Cell(CellType type, std::optional<Ship> ship)
        : _type{type}, _ship{std::forward<std::optional<Ship>>(ship)} {
      if (type & UNDAMAGED_SHIP && !_ship) {
        throw std::logic_error("Cell with ship but no ship_id");
      } else if (!(type & UNDAMAGED_SHIP) && _ship) {
        throw std::logic_error("Cell without undamaged ship but with ship_id");
      }
    }

    //Getters
    std::optional<Ship> ship() const { return _ship; }
    constexpr inline CellType type() const { return _type; }

    //Setters
    void setShip(Ship ship) { _ship = ship; }
    void setType(CellType newType) { _type = newType; }
};
