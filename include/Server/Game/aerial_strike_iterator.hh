#pragma once

#include "board_coordinates.hh"

class AerialStrikeIterator {
  private:
    BoardCoordinates _coords;
    size_t _i;
    size_t _j;
    bool _top_right{false};

  public:
    AerialStrikeIterator(BoardCoordinates coords)
        : _coords{coords}, _i{0}, _j{0} {}

    BoardCoordinates operator*() const { return _coords + BoardCoordinates(_i, _j); }

    AerialStrikeIterator &operator++() {
      if (!_top_right) {
        if (_i < 3) {
          _i++;
        } else if (_j < 2) {
          _j++;
        } else {
          _top_right = true;
          _i = 0;
          _j = 1;
        }
      } else {
        if (_j < 3) {
          _j++;
        } else {
          _i++;
        }
      }
      return *this;
    }

    bool operator==(const AerialStrikeIterator &other) const {
      return _i == other._i && _j == other._j;
    }

    bool operator!=(const AerialStrikeIterator &other) const {
      return _i + _coords.x() != other._i + other._coords.x() || _j + _coords.y() != other._j + other._coords.y();
    }
};
