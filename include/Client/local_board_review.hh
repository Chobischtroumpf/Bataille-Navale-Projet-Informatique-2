#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "player.hh"
#include "game_view.hh"
#include "game_client.hh"
#include "cell.hh"
#include "cell_type.hh"
#include "board_coordinates.hh"



/**
 * Local board view for replay game.
*/
class LocalBoardReview : public GameView {
    private:
        const std::string _session_id;
        std::vector<std::vector<Cell>> _my_board;
        std::vector<std::vector<Cell>> _their_board;

    public:

        /**
         * Constructor/Destructor
         */
        LocalBoardReview(const std::string &session_id);
        ~LocalBoardReview() override = default;

        /**
         * Update the board with a move.
         * @param new_board: the board with the execute move.
         */
        void update_board(const nlohmann::json &new_board);

        /**
         * Find the cell type corresponding to an given type string.
         * @param type: the the in string.
         * @return: the corrsponding CellType.
         */
        CellType string_to_celltype(const std::string &type);

        /**
         * Get the width of the board
         * @return: the board's width
         */
        std::size_t width() const override;
        
        /**
         * Get the height of the board
         * @return: the board's height
         */
        std::size_t height() const override;

        const std::string getSessionId();

        CellType best(CellType lhs, CellType rhs);

        CellType cellType(bool my_side, BoardCoordinates coordinates) const override;

        Cell get(bool my_side, BoardCoordinates position) const;


        bool isFinished() const override{return false;}
        bool isVictory() const override{return false;}
        bool isSameShip(bool my_side, BoardCoordinates first, BoardCoordinates second) const override{return false;}
        bool myTurn() const {return false;}


};