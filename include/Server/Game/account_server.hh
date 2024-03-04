#pragma once

#include <string>

using std::string;

/* 
 * Collecte des données de la base de donnée
*/
class AccountServer {
public:
    bool createAccount(string username, string password);
    bool checkUser(string, string password);
};