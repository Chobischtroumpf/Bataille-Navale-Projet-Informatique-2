#pragma once

#include <map>
#include <chrono>
#include <string>
#include <random>

using std::string;
using std::map;
using namespace std::chrono;

/*
 * TimePoint is a time_point with a steady_clock and a duration of seconds
*/
typedef time_point<steady_clock, seconds> TimePoint;

/*
 * class handles the user's authentication tokens, keeps them in a map
*/

class TokenHandler{
    private:
        map<string /*token*/, std::pair<string /*userID*/, TimePoint/*time until expiration*/>> valid_tokens;

        string findToken(string user_id);

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
        string generateToken(string user_id);

        /*
         *removes the user_id/token pair from the the map
        */
       bool removeToken(string token="");

        /*
         * checks if the user_id and token pair are still valid, returns true if they are,
         * or false if they are'nt
        */
        bool validateToken(string token);

        /*
         * will update user's token validity to the amount of time
         * in minutes specified in update_validity, defaults to 15 minutes
        */
        bool updateTokenValidity(string token, int update_validity=15);

        /*
         * returns the user_id of the token
        */
        string getUserID(string token);
        
};
