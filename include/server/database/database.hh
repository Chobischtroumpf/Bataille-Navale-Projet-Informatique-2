#ifndef DATABASE_HH
#define DATABASE_HH

#include "query_result.hh"

#include <sqlite3.h>
#include <iostream>
#include <vector>


class DataBase
{
private:

    sqlite3 *db; // the pointer to the sqlite database connexion
    /**
    * Establish a connection with SQLite3 and create the db if it do not already exists.
    */
    DbError createDb();

    /**
    * Create a database's tables from a ddl script.
    */
    DbError createTables();

    /**
    * Print the result of a query.
    * @param data: generic pointer to additional data
    * @param argc: the number of columns in the result set
    * @param argv: the values in the current row of the result set
    * @param az_col_name: the names of the columns in the result set
    */
    static int callback(void *data, int argc, char **argv, char **az_col_name);

    /**
    * Execute a sql query, use the callback method to fetch the results.
    * @param sql_query: the query to execute
    * @return A QueryResult struct containing the result of the query andupdate
    * the 'error' field indicates any error that occurred during the operation.     
    */
    QueryResult executeQuery(std::string& sql_query);

    /**
     * Determine the type of error based on the error message returned by SQLite.
     * @param errorMsg The error message returned by SQLite.
     * @return The type of error as a DbError enum value.
     */
    DbError parseError(const std::string& errorMsg);


public:

    /**
    * Constructor, 
    * createDb and createTable do not alter the db if it already exists.
    */
    DataBase(){
        createDb();
        createTables();
    };

    /**
    * Destructor
    */
    ~DataBase(){closeConnection();};

    /**
    * Insert a new entry into the specified table with the given column names and values.
    * @param table_name The name of the table to insert into.
    * @param columns A comma-separated list of column names.
    * @param values A comma-separated list of corresponding values for the columns.
    * @return A QueryResult struct containing the result of the query andupdate
    * the 'error' field indicates any error that occurred during the operation.      
    */
    QueryResult insertEntry(const std::string &table_name, const std::string& columns, const std::string& values);

    /**
    * Delete an entry of the data base.
    * @param table_name: the name of the table where is the entry to delete.
    * @param condition_column: Name of the column to use as a condition.
    * @param condition_value: Value of the condition column.
    * @return A QueryResult struct containing the result of the query andupdate
    * the 'error' field indicates any error that occurred during the operation.     */
    QueryResult deleteEntry(const std::string &table_name, const std::string& condition);

    /**
    * Update an entry in the database.
    * @param table_name: Name of the table to update.
    * @param columns: Name of the column to update and its new value.
    * @param condition: Name of the column to use as a condition.
    * @return A QueryResult struct containing the result of the query andupdate
    * the 'error' field indicates any error that occurred during the operation. 
    */
    QueryResult updateEntry(const std::string& table_name, const std::string& columns, const std::string& condition);

    /**
    * Perform a SELECT query on the database.
    * @param columns: Comma-separated list of columns to retrieve.
    * @param table_name: Name of the table to query.
    * @param condition: Condition     
    * @return A QueryResult struct containing the result of the query andupdate
    * the 'error' field indicates any error that occurred during the operation.     
    */
    QueryResult selectFromTable(const std::string& table_name, const std::string& columns, const std::string& condition);

    /**
    * Rollback the transaction if an error occurs
    */
    void rollBack();

    /**
    * Close the db connection
    */
    void closeConnection();
};



#endif