#ifndef QUERIES_HH
#define QUERIES_HH

#include"database.hh"

#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <memory>

class Queries
{
private:

    // Pointer to the database object
    std::shared_ptr<DataBase> db;

public:

    /**
    * Constructor, 
    */
    Queries(std::shared_ptr<DataBase> db){ this->db = db;}

    /**
     * Check if the provided password matches the password associated with the specified user.
     * @param id_user The ID of the user whose password is to be checked.
     * @param pwd The password to be checked.
     * @return True if the provided password matches the password associated with the user ID, false otherwise.
     */
    bool checkPwd(const std::string &id_user, const std::string &pwd);

    /**
     * Check if the provided username exists.
     * @param username The the username to be checked.
     * @return the result of the request.
     */
    QueryResult checkUserName(const std::string &username);

    /**
    * Authenticate a user with the provided username and password.
    * @param username The username of the user.
    * @param pwd The password of the user.
    * @return True if the user is successfully authenticated, false otherwise.
    */
    QueryResult userLogin(const std::string &username, const std::string &pwd);

    /**
    * Register a new user with the provided username and password.
    * @param username The username of the new user.
    * @param pwd The password of the new user.
    * @return A QueryResult with the id_user of the new user.
    */
    QueryResult userRegister(const std::string &username, const std::string &pwd);

    /**
    * Send a message from one user to another.
    * @param id_sender The ID of the sender.
    * @param id_receiver The ID of the receiver.
    * @param msg The message to be sent.
    * @return True if the message is successfully sent, false otherwise.
    */
    QueryResult sendMsg(const std::string &id_sender, const std::string &id_receiver, const std::string &msg);

    /**
    * Add a friend to the user's friend list.
    * @param id_user The ID of the user.
    * @param friend_username The username of the friend to be added.
    * @return True if the friend is successfully added, false otherwise.
    */
    QueryResult addFriend(const std::string &id_user, const std::string &friend_username);

    /**
    * Get all messages exchanged between two users.
    * @param id_user The ID of the first user.
    * @param id_friend The ID of the second user.
    * @return A QueryResult containing the messages exchanged between the two users.
    */
    QueryResult getMsgBetweenUsers(const std::string &id_user, const std::string &id_friend);

    /**
    * Get all messages exchanged by a user.
    * @param id_user The ID of the user.
    * @return A QueryResult containing all messages exchanged by the user.
    */
    QueryResult getAllUserMsg(const std::string &id_user);


    /**
    * Get the username.
    * @param id_user The ID of the user.
    * @return A QueryResult containing all messages exchanged by the user.
    */
    QueryResult getUsername(const std::string &id_user);


    /**
    * Update the username of a user.
    * @param id_user The id of the user.
    * @param pwd The password of the user.
    * @param new_username The new username to be set.
    * @return True if the username is successfully updated, false otherwise.
    */
    QueryResult updateUserName(const std::string &username, const std::string &pwd, const std::string &new_username);

    /**
    * Update the password of a user.
    * @param username The username of the user.
    * @param pwd The current password of the user.
    * @param new_pwd The new password to be set.
    * @return True if the password is successfully updated, false otherwise.
    */
    QueryResult updatePwd(const std::string &username, const std::string &pwd, const std::string &new_pwd);

    /**
     * Delete a friend from the user's friend list.
     * @param username The username of the user.
     * @param friend_username The username of the friend to be deleted.
     * @return True if the friend is successfully deleted, false otherwise.
     */
    QueryResult deleteFriend(const std::string &username, const std::string &friend_username);
    
    /**
    * Get the current time as a string.
    * @return A string representing the current time.
    */
    std::string getTime();

    /**
     * Deletes the user's account from the database.
     * @param id_user The ID of the user whose account is to be deleted.
     * @param pwd The password of the user for authentication.
     * @return True if the account is successfully deleted, false otherwise.
     */
    QueryResult deleteAccount(const std::string &id_user, const std::string &pwd);

    /**
     * Select a user's friend list.
     * @param id_user The ID of the user whose account is to be deleted.
     * @return A QueryResult containing the friend list.
     */
    QueryResult getUserFriends(const std::string &id_user);

    // for test only
    void printSelectUsers();
    // for test only
    void printSelectRelations();
    // for test only
    void printSelectMessages();
};

#endif