#pragma 

#include <memory>
#include "../../Views/friend_list_view.hh"
#include "../console.hh"

class FriendListConsole : public Console {
public:

private:
    std::shared_ptr<FriendListView> _view;
};