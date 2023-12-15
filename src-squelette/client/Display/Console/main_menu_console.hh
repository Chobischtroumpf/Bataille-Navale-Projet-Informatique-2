#pragma once

#include <memory>
#include "../../Views/main_menu_view.hh"
#include "../console.hh"

class MainMenuConsole : public Console {
public:
private:
  std::shared_ptr<MainMenuView> _view;
};