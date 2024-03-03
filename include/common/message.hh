#pragma once

#include <string>

/*
 * Représentation d'un message
 */
class Message {
public:
    Message(const std::string& text, const std::string& user);

    const std::string& getText() const;
    const std::string& getSender() const;
    const std::string& getDestination() const;

private:
    std::string _text;
    std::string _sender;
    std::string _destination;
};