#ifndef SECURITY_HH
#define SECURITY_HH

#include <iostream>
#include <openssl/sha.h>

namespace Security {

    /**
     * Generates a salt for password hashing.
     * @return The salt as a hex string.
     */
    std::string genSalt();

    /**
    * Hashes a password using a salt.
    * @param salt_pwd The password to hash.
    * @return the hashed password as a hex string.
    */
    std::string hashPwd(const std::string& salt_pwd);

    /**
    * Check if a given salted password match a given hash password.
    * @param hash_pwd The hashed password.
    * @param salt_pwd the salted password.
    * @return true if the two password match.
    */
    bool verifyPwd(const std::string& hash_pwd, const std::string& salt_pwd);
}


#endif