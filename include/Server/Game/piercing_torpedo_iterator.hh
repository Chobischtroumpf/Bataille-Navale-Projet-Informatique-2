#pragma once

#include "board_coordinates.hh"

class PiercingTorpedoIterator {
  private:
    BoardCoordinates _coords;
    size_t _i;

  public:
    PiercingTorpedoIterator(BoardCoordinates coords, size_t width, size_t height)
        : _coords{coords}, _i{0} {}

    BoardCoordinates operator*() const { return _coords + BoardCoordinates(0, _i); }

    PiercingTorpedoIterator &operator++() {
      ++_i;
      return *this;
    }

    bool operator==(const PiercingTorpedoIterator &other) const { return _i == other._i; }

    bool operator!=(const PiercingTorpedoIterator &other) const {
      return _i + _coords.y() != other._i + other._coords.y();
    }
};

