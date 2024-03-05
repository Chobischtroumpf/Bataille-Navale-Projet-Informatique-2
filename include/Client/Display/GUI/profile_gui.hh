#pragma once

#include <memory>

#include "profile_view.hh"
#include "gui.hh"

class ProfileGUI : GUI {
private:
  std::shared_ptr<ProfileView> _view;
};