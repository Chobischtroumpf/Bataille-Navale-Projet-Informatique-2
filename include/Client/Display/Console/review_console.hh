#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

#include "console.hh"
#include "game_client.hh"
#include "review_controller.hh"



class ReviewConsole: public Console{
    private:
        std::ostream &_out;
        std::istream &_in;
        std::shared_ptr<ReviewController> const _controller;
    
    public:
    /** 
    * Contructor
    * @param out: where to print
    * @param in: from where to read
    * @param controller: who to inform of user actions.
    */
    ReviewConsole(std::ostream &out, std::istream &in, std::shared_ptr<ReviewController> controller);
    ~ReviewConsole() = default;

    /**
    * Display the menu of the selection of the game to review.
    * @param session_id_list: the list of the game to review.
    */
    void displayGameList();

    /**
    * Handle the input of the menu.
    */
    ReturnInput handleInput() override ;

    /**
    * Handle the display.
    */
    void display() override;

    void displayError() override {}
    void update() override {}

};

