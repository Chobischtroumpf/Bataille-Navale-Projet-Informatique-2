#include <iostream>

#include "../../include/common/board_coordinates.hh"

std::ostream& operator<<(std::ostream& os, const BoardCoordinates& bc) {
  os << bc.toString();
  return os;
}

std::istream& operator>>(std::istream& is, BoardCoordinates& bc) {
  std::string x_string;
  while (is.good() && !is.eof() &&
         BoardCoordinates::isalpha(static_cast<char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    x_string += c;
  }

  std::string y_string;
  while (is.good() && !is.eof() && std::isdigit(static_cast<unsigned char>(is.peek()))) {
    char c{'?'};
    is.get(c);
    y_string += c;
  }

  std::optional<size_t> x{BoardCoordinates::parseX(x_string)};
  std::optional<size_t> y{BoardCoordinates::parseY(y_string)};

  if (x && y) {
    bc.set(x.value(), y.value());
  } else {
    is.setstate(std::ios_base::failbit);
  }

  return is;
}
