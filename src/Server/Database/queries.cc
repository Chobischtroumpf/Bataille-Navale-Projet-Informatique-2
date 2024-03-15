#include "queries.hh"
#include "database.hh"
#include "security.hh"

#include <iostream>

QueryResult Queries::getUsername(const std::string &id_user){
    std::string condition = "id_user = '" + id_user + "'";
    QueryResult result = db->selectFromTable("Users", "username", condition);
    if(result.data.empty()){result.error = DbError::NON_EXISTENT_USER_ID;}
    return result;
}


QueryResult Queries::checkUserName(const std::string &username){
    std::string condition = "username = '" + username + "'";
    QueryResult result = db->selectFromTable("Users", "id_user", condition);
    if(result.data.empty()){result.error = DbError::NON_EXISTENT_USER_NAME;}
    return result;
}


bool Queries::checkPwd(const std::string &id_user, const std::string &pwd){
    // fetch user data
    std::string columns = "hash_pwd, salt";
    std::string condition = "id_user = '" + id_user + "'";
    QueryResult result = db->selectFromTable("Users", columns, condition);
    if (result.error != DbError::OK || result.data.size() == 0) {
        return false;   // Id_user do not exists
    }
    // Parse result data
    std::string hash_pwd = result.data[0][0];
    std::string salt = result.data[0][1];
    std::string salt_pwd = pwd + salt;
    // check if the password match 
    if (!Security::verifyPwd(hash_pwd, salt_pwd)){
        return false;   // Wrong password!
    }
    return true;
}


bool Queries::userLogin(const std::string &username, const std::string &pwd){
    QueryResult result = checkUserName(username);
    if(result.isOk()){
        return checkPwd(result.getFirst(), pwd);
    }
    return false;
}


bool Queries::userRegister(const std::string &username, const std::string &pwd){
    std::string salt = Security::genSalt();
    std::string hashpwd = Security::hashPwd(pwd+salt);
    std::string columns = "username, hash_pwd, salt";
    std::string values = "'" + username + "', '" + hashpwd + "', '" + salt + "'";
    QueryResult result = db->insertEntry("Users", columns, values);
    if(result.isOk()){
        return true;
    }
    return false;
}


bool Queries::addFriend(const std::string &id_user, const std::string &friend_username){
    QueryResult result = checkUserName(friend_username);
    if(result.data.size() != 0){
        std::string id_friend = result.data[0][0];
        std::string columns = "id_user_r, id_friend";
        std::string values = "'" + id_user + "', '" + id_friend + "'";
        result = db->insertEntry("Relations", columns, values);
        if (result.error == DbError::OK){
            values = "'" + id_friend + "', '" + id_user + "'";
            result = db->insertEntry("Relations", columns, values);
        }
    }
    return result.isOk();
}


bool Queries::sendMsg(const std::string &sender, const std::string &receiver, const std::string &msg){
    // Get the string time the message was send (now)
    std::string time = getTime();

    // Insert the message in the db
    std::string clean_msg = std::regex_replace(msg, std::regex("'"), "''");     // Escape single quotes in the message
    std::string columns = "sender, receiver, msg, msg_date_time";
    std::string values = "'" + sender + "', '" + receiver + "', '" + clean_msg + "', '" + time +  "'";
    QueryResult result = db->insertEntry("Messages", columns, values);
    return result.isOk();
}


QueryResult Queries::getMsgBetweenUsers(const std::string &id_user, const std::string &id_friend){
    std::string columns = "sender, receiver, msg";
    std::string condition = "(sender = '" + id_user + "' AND receiver = '" + id_friend + "')"\
                            " OR (sender = '" + id_friend + "' AND receiver = '" + id_user + "')";
    std::string order_by = " ORDER BY msg_date_time";   // Order by the msg_date_time column
    QueryResult result = db->selectFromTable("Messages", columns, condition + order_by);
    return result;
}


QueryResult Queries::getAllUserMsg(const std::string &id_user){
    std::string columns = "sender, receiver, msg";
    std::string condition = "(sender = '" + id_user + "' OR receiver = '" + id_user + "')";
    std::string order_by = " ORDER BY (CASE WHEN sender = '" + id_user + "' THEN receiver ELSE sender END)";
    QueryResult result = db->selectFromTable("Messages", columns, condition + order_by);
    return result;
}

std::string Queries::getTime(){
    std::time_t now = std::time(nullptr);
    std::tm *tm_time = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", tm_time);
    return std::string(buffer);
}


QueryResult Queries::updateUserName(const std::string &id_user, const std::string &pwd, const std::string &new_username){
    QueryResult result;
    // Check if the provided password is correct
    if (checkPwd(id_user, pwd)){
        // Update the username for the specified user
        std::string set_clause = "username = '" + new_username + "'";
        std::string condition = "id_user = '" + id_user + "'";
        result = db->updateEntry("Users", set_clause, condition);
    }
    else{result.error = DbError::EXECUTION_ERROR;}
    return result;
}


QueryResult Queries::updatePwd(const std::string &id_user, const std::string &pwd, const std::string &new_pwd) {
    QueryResult result;
    // Check if the password is correct
    if (checkPwd(id_user, pwd)) {
        // Update the password for the given user
        std::string salt = Security::genSalt();
        std::string nhpwd = Security::hashPwd(new_pwd+salt);
        std::string set_clause = "hash_pwd = '" + nhpwd + "', salt = '" + salt + "'";
        std::string condition = "id_user = '" + id_user + "'";
        result = db->updateEntry("Users", set_clause, condition);
    }
    else{result.error = DbError::EXECUTION_ERROR;}
    return result;
}


QueryResult Queries::deleteFriend(const std::string &id_user, const std::string &id_friend) {
    std::string condition = "(id_user_r = '" + id_user + "' AND id_friend = '" + id_friend + "')"\
                            " OR (id_user_r = '" + id_friend + "' AND id_friend = '" + id_user + "')";
    QueryResult result = db->deleteEntry("Relations", condition);
    return result;
}


QueryResult Queries::deleteAccount(const std::string &id_user, const std::string &pwd) {
    QueryResult result;
    // Check if the provided password is correct
    if (checkPwd(id_user, pwd)) {
        // Delete the user's account
        std::string condition = "id_user = '" + id_user + "'";
        result = db->deleteEntry("Users", condition);
    }
    else{result.error = DbError::EXECUTION_ERROR;}

    return result;
}


QueryResult Queries::getUserFriends(const std::string &id_user){
    std::string condition = "id_user_r = '" + id_user + "'";
    QueryResult result = db->selectFromTable("Relations", "id_friend", condition);
    return result;
}


// for test only
void Queries::printSelectUsers(){
    QueryResult r = db->selectFromTable("Users", "*", "");
    for (const auto& row: r.data){
        for (const auto& item : row) {
        std::cout << item << std::endl;
        }
    }
}

// for test only
void Queries::printSelectRelations(){
    QueryResult r = db->selectFromTable("Relations", "*", "");
    for (const auto& row: r.data){
        for (const auto& item : row) {
        std::cout << item << std::endl;
        }
    }
}

// for test only
void Queries::printSelectMessages(){
    QueryResult r = db->selectFromTable("Messages", "*", "");
    for (const auto& row: r.data){
        for (const auto& item : row) {
        std::cout << item << std::endl;
        }
    }
}