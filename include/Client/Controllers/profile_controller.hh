#pragma once

#include "controller.hh"

class ProfileController : public Controller {
public:
  void getInfo(std::string username);
  bool modifyPassword(std::string newPassword);
  bool modifyUsername(std::string newName);
};