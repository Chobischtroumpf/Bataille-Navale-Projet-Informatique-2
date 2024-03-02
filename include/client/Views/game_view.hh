#pragma once

#include "../view.hh"
#include "../../../src/common/cell_type.hh"
#include "../../../src/common/board_coordinates.hh"


class GameView : public View {
public:
    virtual ~GameView() override {}

    virtual bool myTurn() const =0;
    virtual bool isFinished() const =0;
    virtual bool isVictory() const =0;
    virtual std::size_t width() const =0;
    virtual std::size_t height() const =0;
    virtual CellType cellType(bool, BoardCoordinates) const = 0;
    virtual bool isSameShip(bool, BoardCoordinates, BoardCoordinates) const =0;
};