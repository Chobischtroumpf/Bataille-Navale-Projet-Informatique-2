#pragma once

#include <optional>
#include "board_view.hh"


class Cell {
    BoardView::CellType     _type{BoardView::WATER};
    std::optional<int>      _ship_id{std::nullopt};

    public:
        /** Default constructor: creates an unkown/Water cell */
        constexpr Cell() = default;
        constexpr Cell(BoardView::CellType type, std::optional<int> ship_id)
            : _type{type}, _ship_id{std::forward<std::optional<int>>(ship_id)} {
        if (type & BoardView::IS_SHIP && !_ship_id) {
            throw std::logic_error("Cell with ship but no ship_id");
        } else if (!(type & BoardView::IS_SHIP) && _ship_id) {
            throw std::logic_error("Cell without ship but with ship_id");
        }
        }
        // Public method to set the type of the cell
        void setType(BoardView::CellType newType) {
            _type = newType;
        }
        [[nodiscard]] constexpr inline BoardView::CellType           type() const { return _type; }
        [[nodiscard]] constexpr inline std::optional<int> shipId() const { return _ship_id; }
};

