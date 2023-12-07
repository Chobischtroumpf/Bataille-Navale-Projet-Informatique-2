#include "ship_coordinates.hh"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <optional>
#include <string>
#include "board_coordinates.hh"

std::istream& operator>>(std::istream& is, ShipCoordinates& sc) {
    std::string x_string;
    while (is.good() && !is.eof() &&
           isalpha(static_cast<char>(is.peek()))) {
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

    std::string r_string;
    while (is.good() && !is.eof() && std::isdigit(static_cast<unsigned char>(is.peek()))) {
        char c{'?'};
        is.get(c);
        r_string += c;
    }

    std::optional<size_t> x = BoardCoordinates::parseX(x_string);
    std::optional<size_t> y = BoardCoordinates::parseY(y_string);

    if (x && y) {
        sc.set(x.value(), y.value());
        if (r_string == "h") {
            sc.setVertical(false);
        } else {
            sc.setVertical(true);
        }
    } else {
        is.setstate(std::ios_base::failbit);
    }

    return is;
}
