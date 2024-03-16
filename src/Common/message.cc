#include "message.hh"

Message::Message(const std::string& text, const std::string& sender)
    : _text(text), _sender(sender) {
}

const std::string& Message::getText() const {
    return _text;
}

const std::string& Message::getSender() const {
    return _sender;
}

