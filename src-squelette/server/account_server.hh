#pragma once

#include "string"
/* 
 * Class that fetch the datas from the database
*/
class AccountServer {
public:
    bool createAccount(std::string username, std::string password);
    bool checkUser(std::string, std::string password);
};