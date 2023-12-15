#pragma once

#include "../../Views/login_view.hh"
#include <memory>
#include "../console.hh"

class LoginConsole : public Console {
public:
private:
  std::shared_ptr<LoginView> _view;
};