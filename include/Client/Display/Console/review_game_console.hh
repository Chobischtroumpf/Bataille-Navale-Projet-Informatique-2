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


class ReviewGameConsole: public Console{
    private:
        std::ostream &_out;
        std::istream &_in;
        std::shared_ptr<LocalBoardReview> const _board;
        std::shared_ptr<ReviewGameController> const _controller;
    
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
    * Display the game review.
    */
    void displayGameReview();
    

    /**
    * Handle the input of the menu.
    */
    ReturnInput handleInput() override ;

    void display() override {}
    void displayError() override {}
    void update() override {}
};

