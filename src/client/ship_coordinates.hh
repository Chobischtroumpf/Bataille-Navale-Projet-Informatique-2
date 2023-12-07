#ifndef GROUP_9_SHIP_COORDINATES_H
#define GROUP_9_SHIP_COORDINATES_H

struct ShipCoordinates {
    BoardCoordinates anchor;
    int length;
    bool vertical;

    // Constructor for initialization
    ShipCoordinates(const BoardCoordinates& anchor, int length, bool vertical)
            : anchor(anchor), length(length), vertical(vertical) {}
};

#endif //GROUP_9_SHIP_COORDINATES_H
