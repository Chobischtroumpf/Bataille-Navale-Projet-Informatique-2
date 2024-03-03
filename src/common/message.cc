#include "../../include/common/message.hh"

Message::Message(const std::string& text, const std::string& user)
    : _text(text), _destination(user) {
}

const std::string& Message::getText() const {
    return _text;
}

const std::string& Message::getSender() const {
    return _sender;
}

const std::string& Message::getDestination() const {
    return _destination;
}