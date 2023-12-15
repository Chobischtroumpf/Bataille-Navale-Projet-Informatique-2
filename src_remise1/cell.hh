#pragma once

#include <optional>
#include "board_view.hh"


class Cell {
    CellType     _type{WATER};
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
        void setId(int id) {
            _ship_id = id;
        }
        [[nodiscard]] constexpr inline CellType           type() const { return _type; }
        [[nodiscard]] constexpr inline std::optional<int> shipId() const { return _ship_id; }
};

