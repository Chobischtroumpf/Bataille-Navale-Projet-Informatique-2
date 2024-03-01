#pragma once

struct Message {
    std::string text;
    std::string sender; // (moi)
    std::string receiver; // L'ami

    Message(const std::string& txt = "", const std::string& sndr = "", const std::string& rcvr = "") 
        : text(txt), sender(sndr), receiver(rcvr) {}
};