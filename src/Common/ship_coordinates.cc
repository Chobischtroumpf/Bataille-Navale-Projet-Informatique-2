#include "ship_coordinates.hh"

std::ostream &operator<<(std::ostream &os, const ShipCoordinates &bc) {
  os << bc.toString();
  return os;
}

std::istream &operator>>(std::istream &is, ShipCoordinates &bc) {

  std::string ship_id_string;
  while (is.good() && !is.eof() &&
         !std::isspace(static_cast<char>(is.peek()))) {
    char c{'?'}; // aucune idee de ce que ca fait, mais ca marche
    is.get(c);
    ship_id_string += c;
  }
  is.ignore(1);

  std::string orientation_string;
  while (is.good() && !is.eof() &&
         !std::isspace(static_cast<char>(is.peek()))) {
    char c{'?'}; // aucune idee de ce que ca fait, mais ca marche
    is.get(c);
    orientation_string += c;
  }
  is.ignore(1);

  std::string x_string;
  while (is.good() && !is.eof() &&
         ShipCoordinates::isalpha(static_cast<char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    x_string += c;
  }

  std::string y_string;
  while (is.good() && !is.eof() &&
         std::isdigit(static_cast<unsigned char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    y_string += c;
  }

  std::optional<ShipType> ship_id{ShipCoordinates::parseShipId(ship_id_string)};
  std::optional<Orientation> orientation{
      ShipCoordinates::parseOrientation(orientation_string)};
  std::optional<size_t> x{ShipCoordinates::parseX(x_string)};
  std::optional<size_t> y{ShipCoordinates::parseY(y_string)};

  if (x && y && orientation && ship_id) {
    bc.set(x.value(), y.value(), orientation.value(), ship_id.value());
  } else {
    is.setstate(std::ios_base::failbit);
  }

  return is;
}
