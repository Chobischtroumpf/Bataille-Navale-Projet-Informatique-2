#pragma once

#include "../../Views/login_view.hh"
#include <memory>
#include "../console.hh"

class RegisterConsole : public Console {
public:
  virtual void display() override {}
  virtual void display_error() override {}
  virtual void update() override {}
  virtual ReturnInput handle_input() override { return ReturnInput(); }
private:
  std::shared_ptr<LoginView> _view;
};