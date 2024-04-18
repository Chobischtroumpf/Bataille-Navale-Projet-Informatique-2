#pragma once

#include <memory>

#include "../console.hh"
#include "../../Views/login_view.hh"

class RegisterConsole : public Console {
private:
  std::shared_ptr<LoginView> _view;

public:
  virtual void display() override {}
  virtual void displayError() override {}
  virtual void update() override {}
  virtual ReturnInput handleInput() override { return ReturnInput(); }
};