#pragma once

#include "../../Views/login_view.hh"
#include <memory>
#include "../gui.hh"

class LoginGUI : public GUI {
public:
private:
  std::shared_ptr<LoginView> _view;
};