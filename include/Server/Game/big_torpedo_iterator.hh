#pragma once

#include "board_coordinates.hh"

class BigTorpedoIterator {
  private:
    BoardCoordinates _coords;
    size_t _i;
    size_t _j;

  public:
    BigTorpedoIterator(BoardCoordinates coords)
      : _coords{coords}, _i{0}, _j{0} {}

    BoardCoordinates operator*() const { return _coords + BoardCoordinates(_i, _j); }

    BigTorpedoIterator &operator++() {
      if (_j < 1) {
      ++_j;
      } else {
      _j = 0;
      ++_i;
      }
      return *this;
    }

    bool operator==(const BigTorpedoIterator &other) const {
      return _i == other._i && _j == other._j;
    }

    bool operator!=(const BigTorpedoIterator &other) const {
      return _i + _coords.x() != other._i + other._coords.x() || _j + _coords.y() != other._j + other._coords.y();
    }
};
