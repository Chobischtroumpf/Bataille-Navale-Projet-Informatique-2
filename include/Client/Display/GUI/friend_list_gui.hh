#pragma once

#include <memory>

#include "friend_list_view.hh"
#include "gui.hh"

class FriendListGui : public GUI {
public:

private:
    std::shared_ptr<FriendListView> _view;
};