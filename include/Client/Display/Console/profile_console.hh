#pragma once

#include <memory>

#include "profile_view.hh"
#include "console.hh"

class ProfileConsole : public Console {
private:
  std::shared_ptr<ProfileView> _view;
};