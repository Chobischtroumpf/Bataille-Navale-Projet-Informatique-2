#pragma once

#include "../../Views/main_menu_view.hh"
#include "../gui.hh"
#include <memory>

class MainMenuGUI : public GUI {
public:
private:
  std::shared_ptr<MainMenuView> _view;
};