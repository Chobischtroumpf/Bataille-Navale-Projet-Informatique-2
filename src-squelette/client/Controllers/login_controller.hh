#pragma once

#include <string>
#include "../controller.hh"

class LoginController : public Controller {
public:
    bool attemptLogin(std::string username, std::string password) const;
private:
    bool checkValidity(std::string username, std::string password) const;
};