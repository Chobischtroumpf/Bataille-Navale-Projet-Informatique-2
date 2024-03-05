#pragma once

#include "controller.hh"

class RegisterController : public Controller {
public:
    bool attemptRegister(std::string username, std::string password) const;
private:
    bool checkValidity(std::string username, std::string password) const;
};