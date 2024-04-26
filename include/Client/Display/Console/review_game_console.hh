#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

#include "console.hh"
#include "game_client.hh"
#include "local_board_review.hh"
#include "review_game_controller.hh"

struct PrintInfo{
    uint8_t letter_width;    // width in a column name
    uint8_t number_width;    // width in a row name
    std::string gap;         // grid separator
    size_t grid_width;       // width of a line of a grid
    size_t width;            // width of a line with two grids
    std::vector<string> map_key;   // string explain cell type, no ending '\n'.
    PrintInfo(uint8_t letter_width_, uint8_t number_width_, std::string gap_, size_t grid_width_, size_t width_, std::vector<string> map_key_)
        : letter_width(letter_width_),number_width(number_width_), gap(std::move(gap_)), 
          grid_width(grid_width_), width(width_), map_key(std::move(map_key_)) {}
};


class ReviewGameConsole: public Console{
    private:
        std::ostream &_out;
        std::istream &_in;
        std::shared_ptr<LocalBoardReview> const _board;
        std::shared_ptr<ReviewGameController> const _controller;
        PrintInfo _print_info;

        /** 
         * Create map key, each string is a line without ending '\n'
         * Used by the constructor to init _map_key.
         * Outside constructor, use the attribute instead. 
         */
        std::vector<string> createMapKey() const;

        /**
         * Return a single character representing a type of cell on the board.
         * @param type: The enum type of the cell.
         */
        static string toString(CellType type);

        /** 
         * Count number of unicode characters in a UTF-8 encoded string (assume linux
         * platform)
         * @param s: The UTF-8 string.
         */
        static constexpr size_t length(const string &s);


    public:
        /** 
        * Contructor
        * @param out: where to print
        * @param in: from where to read
        * @param board: what to print, must inherit from BoardView
        * @param controller: who to inform of user actions.
        * WARNING: ConsoleBoardDisplay does not support a change in board->height()
        * or board->width() after construction. 
        */
        ReviewGameConsole(std::ostream &out, std::istream &in, std::shared_ptr<LocalBoardReview> board, 
                        std::shared_ptr<ReviewGameController> controller);
        ~ReviewGameConsole() = default;
        
        /**
        * Handle the input of the menu.
        */
        ReturnInput handleInput() override;

        /**
        * Console methods.
        */
        void display() override;
        void displayError() override {}
        void update() override {}

        /***************
        **Print Methods**
        ****************/

        /**
        * Display the header of this menu.
        */
        void displayHeader();

        /**
        * Display the game review boards.
        */
        void displayGameReview();

        /**
        * Display the descrition of the icones 
        * used for the cell types.
        */
        void displayMapKey();

        /**
        * Display the possible actions of user input.
        */
        void displayActions();

        /**
         * Display two boards side by side.
         * @param left: Left board.
         * @param right: Right board. 
         */
        void printSideBySide(std::vector<string> left, std::vector<string> right);

        /*******************
        **Pre_print Methods**
        ********************/

        /**
         * Create the letters indexing the board.
         * @param oss: stream
         */
        void createLetters(std::ostringstream& oss);

        /**
         * Create the first line of the board.
         * @param oss: stream
         */
        void createFirstLine(std::ostringstream& oss);

        /**
         * Create the body of the board.
         * @param oss: stream
         * @param i: the line to print
         * @param my_side: user side or not
         */
        void createBody(std::ostringstream& oss, int i, bool my_side);

        /**
         * Create the last line of the board.
         * @param oss: stream
         */
        void createLastLine(std::ostringstream& oss);

        /**
         * Call the methods necessary to build a complete board.
         * @param my_side: user side or not
         */
        std::vector<string> createGrid(bool my_side);


};

