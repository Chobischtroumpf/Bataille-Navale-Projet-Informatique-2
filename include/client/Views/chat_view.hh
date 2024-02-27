#pragma once

#include <vector>
#include <string>

#include "../view.hh"
#include "../../common/message.hh"


class ChatView : public View {
public:
	 std::vector<Message> get_messages(const std::vector<Message>& messages);
private:
    std::vector<Message> messages;
};
