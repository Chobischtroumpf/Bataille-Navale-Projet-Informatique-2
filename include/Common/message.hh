#pragma once

#include <string>

/*
 * Représentation d'un message
 */
class Message {
private:
    std::string _text;
    std::string _sender;

public:
    Message(const std::string& text, const std::string& sender);

    const std::string& getText() const;
    const std::string& getSender() const;
};