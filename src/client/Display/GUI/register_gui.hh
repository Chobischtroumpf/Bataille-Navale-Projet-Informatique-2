#pragma once

#include <memory>
#include "../../Views/register_view.hh"
#include "../gui.hh"

class RegisterGUI : public GUI {
public:
private:
  std::shared_ptr<RegisterView> _view;
};