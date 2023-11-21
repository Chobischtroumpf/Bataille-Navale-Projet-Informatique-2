#pragma once

#include <stdexcept>
#include <string>

using std::string;

/** Exception class to indicate that a feature has not been (yet) implemented */
class NotImplementedError : public std::logic_error {
 public:
  explicit NotImplementedError(const string& what_arg)
      : std::logic_error("Not implemented: " + what_arg) {}
};
