#pragma once

#include "board_coordinates.hh"
#include "not_implemented_error.hh"
#include "ship_types.hh"
#include <optional>
#include <string>

using std::string;

typedef enum { VERTICAL, HORIZONTAL } Orientation;

/*
 * Représente une position de bateau, son type, et si il est mis en vertical ou
 * horizontal
 */
class ShipCoordinates final : public BoardCoordinates {
  Orientation _orientation;
  ShipType _ship_id;

public:
  // Default constructor
  ShipCoordinates()
      : BoardCoordinates{}, _orientation{HORIZONTAL}, _ship_id{NONE} {}

  ShipCoordinates(size_t x, size_t y, Orientation orientation)
      : BoardCoordinates{x, y}, _orientation{orientation} {}

  [[nodiscard]] constexpr inline Orientation orientation() const {
    return _orientation;
  }
  [[nodiscard]] constexpr inline ShipType ship_id() const { return _ship_id; }

  void set(size_t x, size_t y, Orientation orientation, ShipType ship_id) {
    _x = x;
    _y = y;
    _orientation = orientation;
    _ship_id = ship_id;
  }

  // Supercharge the == operator
  bool operator==(const ShipCoordinates &other) const {
    return x() == other.x() && y() == other.y() &&
           orientation() == other.orientation() && ship_id() == other.ship_id();
  }

  [[nodiscard]] static std::optional<ShipType>
  parseShipId(const string &ship_id_string) {
    if (ship_id_string.empty()) {
      return std::nullopt;
    }
    try {
      int ship_id = std::stoi(ship_id_string);
      if (ship_id < 0 || ship_id > 5) {
        return std::nullopt;
      }
      return ShipType(ship_id);
    } catch (std::invalid_argument &) {
      return std::nullopt;
    } catch (std::out_of_range &) {
      return std::nullopt;
    }
  }

  [[nodiscard]] static std::optional<Orientation>
  parseOrientation(const string &orientation_string) {
    if (orientation_string.empty() || orientation_string.length() > 1) {
      return std::nullopt;
    }
    if (orientation_string == "H" || orientation_string == "h") {
      return HORIZONTAL;
    } else if (orientation_string == "V" || orientation_string == "v") {
      return VERTICAL;
    } else {
      return std::nullopt;
    }
  }

  /** {0, 0} returns "A1" */
  [[nodiscard]] inline string toString() const override {
    return shipIdToString() + orientationToString() + xToString() + yToString();
  }

  // Returns the orientation as a string
  [[nodiscard]] inline string orientationToString() const {
    if (_orientation == HORIZONTAL) {
      return "H";
    } else {
      return "V";
    }
  }

  // Returns the ship id as a string
  [[nodiscard]] inline string shipIdToString() const {
    switch (_ship_id) {
    case CARRIER:
      return "Carrier";
    case BATTLESHIP:
      return "Battleship";
    case SUBMARINE:
      return "Submarine";
    case DESTROYER:
      return "Destroyer";

    case NONE:
    default:
      return "0";
    }
  }
};

/** Put bc.toString() on os */
std::ostream &operator<<(std::ostream &os, const ShipCoordinates &bc);

/** Extract bc from os */
std::istream &operator>>(std::istream &is, ShipCoordinates &bc);
