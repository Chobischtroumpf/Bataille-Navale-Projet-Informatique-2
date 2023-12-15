#pragma once

#include <optional>
#include "board_view.hh"

//Class representing a tile of the board (cell)
class Cell {
    //Current type of the cell
    CellType     _type{WATER};
    //Id of the cell, representing the ShipType it belongs to
    std::optional<int>      _ship_id{std::nullopt};

    public:
        /** Default constructor: creates an unkown/Water cell */
        constexpr Cell() = default;
        constexpr Cell(CellType type, std::optional<int> ship_id)
            : _type{type}, _ship_id{std::forward<std::optional<int>>(ship_id)} {
        if (type & IS_SHIP && !_ship_id) {
            throw std::logic_error("Cell with ship but no ship_id");
        } else if (!(type & IS_SHIP) && _ship_id) {
            throw std::logic_error("Cell without ship but with ship_id");
        }
        }
        // Public method to set the type of the cell
        void setType(CellType newType) {
            _type = newType;
        }
        // Public method to set the id of the cell
        void setId(int id) {
            _ship_id = id;
        }
        // Public method to get the type of the cell
        [[nodiscard]] constexpr inline CellType           type() const { return _type; }
        // Public method to get the ship id of the cell
        [[nodiscard]] constexpr inline std::optional<int> shipId() const { return _ship_id; }
};

