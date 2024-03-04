#pragma once

#include <memory>

#include "login_view.hh"
#include "gui.hh"

class LoginGUI : public GUI {
public:
private:
  std::shared_ptr<LoginView> _view;
};