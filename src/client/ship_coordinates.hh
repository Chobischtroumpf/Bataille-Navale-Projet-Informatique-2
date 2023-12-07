#ifndef GROUP_9_SHIP_COORDINATES_H
#define GROUP_9_SHIP_COORDINATES_H

#include <optional>
#include <string>
#include "board_coordinates.hh"

class ShipCoordinates {
    BoardCoordinates anchor;
    int length;
    bool vertical;
public:
    // Constructor for initialization
    ShipCoordinates(const BoardCoordinates& anchor, int length, bool vertical)
            : anchor(anchor), length(length), vertical(vertical) {}

    ShipCoordinates(int length) : length{length}, vertical{true} {}

    virtual int x() const {return anchor.x();}
    virtual int y() const {return anchor.y();}

    virtual void set(int x, int y) {anchor.set(x, y);}

    virtual BoardCoordinates getAnchor() const {return anchor;}
    virtual int getLength() const {return length;}
    virtual bool getVertical() const {return vertical;}
    virtual bool setVertical(bool new_vertical) {vertical = new_vertical;}

    constexpr static bool isalpha(char c) {
        // Contrary to std::isalpha, works with char and not modified by locale
        return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
    }

};

std::istream& operator>>(std::istream& is, ShipCoordinates& sc);

#endif //GROUP_9_SHIP_COORDINATES_H

