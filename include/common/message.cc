#include "message.hh"

Message::Message(const std::string& text, const std::string& sender, const std::string& destination)
    : _text(text), _sender(sender), _destination(destination) {
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