#pragma once

#include "../../Views/profile_view.hh"
#include "../console.hh"
#include <memory>

class ProfileConsole : public Console {
private:
  std::shared_ptr<ProfileView> _view;
};