#pragma 

#include <memory>
#include "../../Views/chat_view.hh"
#include "../console.hh"

class ChatConsole : public Console {
public:

private:
    std::shared_ptr<ChatView> _view;
};