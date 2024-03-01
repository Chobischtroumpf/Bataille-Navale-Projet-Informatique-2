#pragma once

#include <map>
#include <chrono>
#include <string>
#include <random>

using std::string;
using std::map;
using namespace std::chrono;

/*
 * 
*/
typedef time_point<steady_clock, seconds> TimePoint;

/*
 * class handles the user's authentication tokens, keeps them  
*/

class TokenHandler{
    private:
        map<string /*token*/, std::pair<int /*userID*/, TimePoint/*time until expiration*/>> valid_tokens;

        string findToken(int user_id);

    public:

        /*
         * constructor/destructor
        */
        TokenHandler() {
            std::srand(system_clock::now().time_since_epoch().count());
        }
        ~TokenHandler() {}
        /*
         * creates a user_id/token kay/value pair that has a base validity of 15 minutes, 
        */
        string generateToken(int user_id);

        /*
         *removes the user_id/token pair from the the map
        */
       bool removeToken(int user_id=0, string token="");

        /*
         * checks if the user_id and token pair are still valid, returns true if they are,
         * or false if they are'nt
        */
        bool validateToken(int user_id=0, string token="");

        /*
         * will update user's token validity to the amount of time
         * in minutes specified in update_validity, defaults to 15 minutes
        */
        bool updateTokenValidity(int user_id, string token, int update_validity=15);
        
};
