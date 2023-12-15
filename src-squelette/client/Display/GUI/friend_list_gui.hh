#pragma once

#include "../../Views/friend_list_view.hh"
#include "../gui.hh"
#include <memory>

class FriendListGui : public GUI {
public:

private:
    std::shared_ptr<FriendListView> _view;
};