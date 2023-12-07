#pragma once

#include <optional>
#include <string>
#include "board_coordinates.hh"

#include "not_implemented_error.hh"
#define VERTICAL true
#define HORIZONTAL false


using std::string;

/** A pair of 0-indexed board coordinates.
 *
 * {0, 0} is top-left.
 *
 * NOTE: This is not the coordinates of a pixel on the screen. */
class ShipCoordinates final: public BoardCoordinates {
  bool _orientation;
 public:
  // Default constructor
  ShipCoordinates() : BoardCoordinates{}, _orientation{HORIZONTAL} {}
  
  ShipCoordinates(size_t x, size_t y, bool orientation) : BoardCoordinates{x,y}, _orientation{orientation} {}

  [[nodiscard]] constexpr inline bool orientation() const { return _orientation; }

  void set(size_t x, size_t y, bool orientation) {
    _x = x;
    _y = y;
    _orientation = orientation;
  }

  //Supercharge the == operator
  bool operator==(const ShipCoordinates& other) const {
        return x() == other.x() && y() == other.y() && orientation() == other.orientation();
    }

  [[nodiscard]] static std::optional<bool> parseOrientation(const string& orientation_string) {
    if (orientation_string.empty() || orientation_string.length() > 1) {
      return std::nullopt;
    }
    
    if (orientation_string == "H" || orientation_string == "h") {
      return HORIZONTAL;
    } else if (orientation_string == "V" || orientation_string == "v"){
      return VERTICAL;
    } else {
      return std::nullopt;
    }
    
  }

  /** {0, 0} returns "A1" */
  [[nodiscard]] inline string toString() const override { return orientationToString() + xToString() + yToString(); }

  //Returns the orientation as a string
  [[nodiscard]] inline string orientationToString() const {
    if (_orientation == HORIZONTAL) {
      return "H";
    } else {
      return "V";
    }
  }
};

/** Put bc.toString() on os */
std::ostream& operator<<(std::ostream& os, const ShipCoordinates& bc);

/** Extract bc from os */
std::istream& operator>>(std::istream& is, ShipCoordinates& bc);

