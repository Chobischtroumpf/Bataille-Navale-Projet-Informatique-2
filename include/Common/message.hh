#pragma once

#include <string>

/*
 * Représentation d'un message
 */
class Message {
public:
    Message(const std::string& text, const std::string& sender);

    const std::string& getText() const;
    const std::string& getSender() const;

private:
    std::string _text;
    std::string _sender;
};