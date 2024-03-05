#pragma once

#include <memory>

#include "chat_view.hh"
#include "gui.hh"

class ChatGui : public GUI {
public:

private:
    std::shared_ptr<ChatView> _view;
};