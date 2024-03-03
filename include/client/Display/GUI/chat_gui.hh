#pragma once

#include "../../Views/chat_view.hh"
#include "../gui.hh"
#include <memory>

class ChatGui : public GUI {
public:

private:
    std::shared_ptr<ChatView> _view;
};