#include "token_handler.hh"

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

bool TokenHandler::removeToken(int user_id, string token){
    if (user_id !=0 && token.length() != 0){
        token = this->findToken(user_id);
    }
    if (token.empty())
        return false;
    else
        this->valid_tokens.erase(token);
    return true;
}

bool TokenHandler::validateToken(int user_id, string token){
    if (token.empty() && user_id != 0){
        token = this->findToken(user_id);
    }
    if (!token.empty()){
        TimePoint current_time = time_point_cast<TimePoint::duration>(steady_clock::now());
        if (this->valid_tokens[token].second >= current_time)
            return true;
    }
    return false;
}

bool TokenHandler::updateTokenValidity(int user_id, string token, int update_validity) {
    TimePoint validity_time = time_point_cast<TimePoint::duration>(steady_clock::now() + minutes(update_validity));
    if (token.empty() && user_id != 0){
        token = this->findToken(user_id);
    }
    if (!token.empty()){
        this->valid_tokens[token].second = validity_time;
        return true;
    }
    else
        return false;
}
