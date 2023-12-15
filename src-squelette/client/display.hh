#pragma once

#include "../common/board_coordinates.hh"


class Display {
public:
    virtual void display() =0;
    virtual void display_error() =0;
    virtual void update() =0;
    virtual void handle_input() =0;
};