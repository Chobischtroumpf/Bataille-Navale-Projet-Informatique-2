#include "TokenHandler.hpp"

string TokenHandler::findToken(int user_id){
    for (auto iter:this->valid_tokens){
        if (iter.second.first == user_id)
            return iter.first;
    }
    return "";
}

string TokenHandler::generateToken(int user_id){
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    string token;

    TimePoint validity_time = time_point_cast<TimePoint::duration>(steady_clock::now() + minutes(15));
    token.resize(32);
    for (int i=0; i < 32; i++)
        token[i] = charset[rand() % charset.length()];
    this->valid_tokens[token] = make_pair(user_id, validity_time);

    return token;
}

bool TokenHandler::removeToken(string token){
    if (token.empty())
        return false;
    else
        this->valid_tokens.erase(token);
    return true;
}

bool TokenHandler::validateToken(string token){
    TimePoint current_time = time_point_cast<TimePoint::duration>(steady_clock::now());
    if (!token.empty()){
        if (this->valid_tokens[token].second >= current_time){
            if (this->valid_tokens[token].second - current_time < minutes(5))
                if (this->updateTokenValidity(token, 15))
                    return true;
            return true;
        } else {
            this->removeToken(token);
            return false;
        }
    }
    return false;
}

bool TokenHandler::updateTokenValidity(string token, int update_validity) {
    TimePoint validity_time = time_point_cast<TimePoint::duration>(steady_clock::now() + minutes(update_validity));
    if (!token.empty()){
        this->valid_tokens[token].second = validity_time;
        return true;
    }
    else {
        return false;
    }
}

int TokenHandler::getUserID(string token){
    if (!token.empty()){
        return this->valid_tokens[token].first;
    }
    return 0;
}
