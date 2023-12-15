#pragma once

#include "../view.hh"
#include "../../common/cell_type.hh"
#include "../../common/board_coordinates.hh"

class GameView : public View {
public:
    virtual bool myTurn() =0;
    virtual bool isFinished() =0;
    virtual bool isVictory() =0;
    virtual std::size_t width() =0;
    virtual std::size_t height() =0;
    virtual CellType cellType(bool, BoardCoordinates) =0;
    virtual bool isSameShip(bool, BoardCoordinates, BoardCoordinates) =0;
};