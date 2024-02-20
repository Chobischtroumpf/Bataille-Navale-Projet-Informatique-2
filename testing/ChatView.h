#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <iostream>
#include <vector>
#include <string>

#include "messages.hh"

class ChatView {
public:
void displayMessage(const std::vector<Message>& messages, std::string selectedUser);
};

#endif // CHATVIEW_H
