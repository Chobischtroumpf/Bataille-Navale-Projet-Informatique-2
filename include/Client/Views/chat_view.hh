#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "view.hh"
#include "message.hh"


class ChatView : public View {
public:
	std::vector<Message> getMessages(const std::vector<Message>& messages);
	void update() override;
private:
    std::vector<Message> messages_;
};
