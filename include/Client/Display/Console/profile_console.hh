#pragma once

#include <memory>

#include "console.hh"
#include "profile_view.hh"

class ProfileConsole : public Console {
private:
  std::shared_ptr<ProfileView> _view;
};