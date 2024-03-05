#pragma once

#include <memory>

#include "main_menu_view.hh"
#include "gui.hh"

class MainMenuGUI : public GUI {
public:
private:
  std::shared_ptr<MainMenuView> _view;
};