#pragma once

#include <vector>
#include <string>

#include "../view.hh"
#include "../../common/message.hh"


class ChatView {
public:
void displayMessage(const std::vector<Message>& messages, std::string selectedUser);
};
