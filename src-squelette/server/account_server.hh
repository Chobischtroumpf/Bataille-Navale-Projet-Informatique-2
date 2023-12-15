#pragma once

#include "string"
/* 
 * Collecte des données de la base de donnée
*/
class AccountServer {
public:
    bool createAccount(std::string username, std::string password);
    bool checkUser(std::string, std::string password);
};