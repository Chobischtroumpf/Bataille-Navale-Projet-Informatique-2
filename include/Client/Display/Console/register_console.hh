#pragma once

#include <memory>

#include "login_view.hh"
#include "console.hh"

class RegisterConsole : public Console {
public:
  virtual void display() override {}
  virtual void displayError() override {}
  virtual void update() override {}
  virtual ReturnInput handleInput() override { return ReturnInput(); }
private:
  std::shared_ptr<LoginView> _view;
};