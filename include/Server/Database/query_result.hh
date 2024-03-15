#pragma once

#include <sqlite3.h>
#include <iostream>
#include <vector>


enum class DbError {
    OK,
    EXECUTION_ERROR,
    DB_CONNECTION_ERROR,
    DDL_FILE_OPENING_ERROR,
    DDL_FILE_EXECUTION_ERROR,
    WRONG_PWD_FAILED,
    NON_EXISTENT_USER_NAME,
    NON_EXISTENT_USER_ID,
    UNIQUE_CONSTRAINT_FAILED,
    CHECK_USERNAME_CONSTRAINT_FAILED,
    CHECK_RELATION_CONSTRAINT_FAILED,
    CHECK_MSG_CONSTRAINT_FAILED,
    CHECK_SENDER_CONSTRAINT_FAILED,
    CANNOT_UPDATE_ID_COLUMN,
    FOREIGN_KEY_CONSTRAINT_FAILED
};


struct QueryResult {
    QueryResult() : error(DbError::OK) {} // Initialize error to DbError::OK
    std::vector<std::vector<std::string>> data;
    DbError error;

    /**
     * Get the first data from the first vector.
     * @return the data if it exists else an empty string.
     */   
    std::string getFirst();

    /**
     * Check if any error happened.
     * @return true if no error, false else.
     */  
    bool isOk();

    /**
     * Translate an error to a string.
     * @return the string error.
     */  
    std::string getError();
};

// for test only
std::ostream& operator<<(std::ostream&, const DbError&);
std::ostream& operator<<(std::ostream&, const QueryResult&);
