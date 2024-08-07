#pragma once

#include <optional>
#include <nlohmann/json.hpp>

#include "not_implemented_error.hh"

/*
 * Représente un coordonnées dans le plateau
 */
class BoardCoordinates {
protected:
  size_t _x;
  size_t _y;

public:
  // Default constructor
  BoardCoordinates() : _x{0}, _y{0} {}

  // Constructor
  BoardCoordinates(size_t x, size_t y) : _x{x}, _y{y} {}

  // Copy constructor
  BoardCoordinates(const BoardCoordinates &other) {
    _x = other._x;
    _y = other._y;
  };

  // Move constructor
  BoardCoordinates(BoardCoordinates &&other) noexcept {
    _x = other._x;
    _y = other._y;
  };

  // Copy assignment
  BoardCoordinates &operator=(const BoardCoordinates &other) {
    if (this != &other) {
      _x = other._x;
      _y = other._y;
    }
    return *this;
  }

  // Destructor
  virtual ~BoardCoordinates() = default;

  [[nodiscard]] constexpr inline size_t x() const { return _x; }
  [[nodiscard]] constexpr inline size_t y() const { return _y; }

  void set(size_t x, size_t y) {
    _x = x;
    _y = y;
  }
  // Supercharge the == operator

  bool operator==(const BoardCoordinates &other) const {
    return x() == other.x() && y() == other.y();
  }

  // Supercharge the + operator

  BoardCoordinates operator+(const BoardCoordinates &other) const {
    return BoardCoordinates(x() + other.x(), y() + other.y());
  }

  /** Whether c is in [A-Za-z] */
  constexpr static bool isalpha(char c) {
    // Contrary to std::isalpha, works with char and not modified by locale
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
  }

  /** "A" -> 0, "B" -> 1, ..., "AA" -> 26, ...
   * Any non alpha character (as defined by BoardCoordinates::isalpha) produces
   * a null result. */
  [[nodiscard]] static std::optional<size_t> parseX(const string &x_string) {
    if (x_string.empty()) {
      return std::nullopt;
    }

    size_t result{0};
    for (char c : x_string) {
      if (!isalpha(c)) {
        return std::nullopt;
      }
      result = result * 26 + static_cast<unsigned>(std::toupper(c) - 'A' + 1);
    }
    return result - 1;
  }

  /** "1" -> 0, "2" -> 1, ...
   * Uses std::stoull. Return null if std::stoull throws an exception. */
  [[nodiscard]] static std::optional<size_t> parseY(const string &y_string) {
    const int BASE{10};
    try {
      unsigned long long parsed = std::stoull(y_string, nullptr, BASE);
      if (parsed == 0) {
        return std::nullopt;
      }
      return parsed - 1;
    } catch (const std::logic_error &) {
      return std::nullopt;
    }
  }

  /** {0, 0} returns "A1" */
  [[nodiscard]] virtual inline string toString() const {
    return xToString() + yToString();
  }

  /** returns the x / letter part of toString() */
  [[nodiscard]] virtual inline string xToString() const {
    std::string result{};
    size_t n = _x + 1;
    while (n > 0) {
      result = static_cast<char>('A' + (n - 1) % 26) + result;
      n = (n - 1) / 26;
    }
    return result;
  }

  /** returns the y / number part of toString() */
  [[nodiscard]] virtual inline string yToString() const {
    return std::to_string(_y + 1);
  }

  nlohmann::json to_json() const {
    nlohmann::json j;
    j["x"] = _x;
    j["y"] = _y;
    return j;
  }

};

/** Put bc.toString() on os */
std::ostream &operator<<(std::ostream &os, const BoardCoordinates &bc);

/** Extract bc from os */
std::istream &operator>>(std::istream &is, BoardCoordinates &bc);
