#pragma once

#include "../../Views/profile_view.hh"
#include "../gui.hh"
#include <memory>

class ProfileGUI : GUI {
private:
  std::shared_ptr<ProfileView> _view;
};