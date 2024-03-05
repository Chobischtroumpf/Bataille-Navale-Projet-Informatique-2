#include "../../../include/server/database/database.hh"
#include "../../../include/server/database/query_result.hh"

#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <ctime>

#define DATABASE_DIR "../src/server/database/DDL_user_db.sql"


DbError DataBase::createDb() {
    int rc = sqlite3_open("users.db", &this->db);
    if (rc) {
        this->db = nullptr;
        closeConnection();
        std::cerr << "create_db failed" << std::endl;
        return DbError::DB_CONNECTION_ERROR;
    }
    // Enable foreign key constraint
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    return DbError::OK;
}


DbError DataBase::createTables(){
    std::cout << "!!!!!!!!!! create tables" << std::endl;
    // Read DDL file
    std::ifstream file(DATABASE_DIR);
    if (!file.is_open()) {
        std::cerr << "open ddl failed" << std::endl;
        return DbError::DDL_FILE_OPENING_ERROR;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sql = buffer.str();

    // Execute DDL file
    int rc = sqlite3_exec(this->db, sql.c_str(), NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(this->db);
        std::cerr << "execute ddl failed" << std::endl;
        return DbError::DDL_FILE_EXECUTION_ERROR;
    }
    return DbError::OK;
}


int DataBase::callback(void *data, int argc, char **argv, char **az_col_name){
    QueryResult *query_result = static_cast<QueryResult*>(data);
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    query_result->data.push_back(row);
    return 0;
}


QueryResult DataBase::executeQuery(std::string& sql_query) {
    QueryResult result;
    int rc = sqlite3_exec(this->db, sql_query.c_str(), callback, &result, NULL);
    if (rc != SQLITE_OK) {
        result.error = parseError(sqlite3_errmsg(this->db));
    }
    else{
        result.error = DbError::OK;
    }
    return result;
}


DbError DataBase::parseError(const std::string& errorMsg) {
    if (errorMsg.find("UNIQUE constraint failed") != std::string::npos) {
        return DbError::UNIQUE_CONSTRAINT_FAILED;
    } 
    else if (errorMsg.find("FOREIGN KEY") != std::string::npos) {
        return DbError::FOREIGN_KEY_CONSTRAINT_FAILED;
    }
    else if (errorMsg.find("username <> ") != std::string::npos) {
        return DbError::CHECK_USERNAME_CONSTRAINT_FAILED;
    } 
    else if (errorMsg.find("id_user_r <> id_friend") != std::string::npos) {
        return DbError::CHECK_RELATION_CONSTRAINT_FAILED;
    } 
    else if (errorMsg.find("sender <> receiver") != std::string::npos) {
        return DbError::CHECK_SENDER_CONSTRAINT_FAILED;
    }
    else if (errorMsg.find("msg <> ''") != std::string::npos) {
        return DbError::CHECK_MSG_CONSTRAINT_FAILED;
    }    
    else if (errorMsg.find("Cannot update id column") != std::string::npos) {
        return DbError::CANNOT_UPDATE_ID_COLUMN;
    }
    return DbError::EXECUTION_ERROR;  // default case
}


QueryResult DataBase::insertEntry(const std::string &table_name, const std::string& columns, const std::string& values){
    std::string sql = "INSERT INTO " + table_name + " (" + columns + ") VALUES (" + values + ");";
    return executeQuery(sql);
}


QueryResult DataBase::deleteEntry(const std::string &table_name, const std::string& condition) {
    std::string sql = "DELETE FROM " + table_name + " WHERE " + condition + ";";
    QueryResult result = executeQuery(sql);
    //int num_del_rows = sqlite3_changes(db);
    return executeQuery(sql);
}


QueryResult DataBase::updateEntry(const std::string& table_name, const std::string& columns, const std::string& condition) {
    std::string sql = "UPDATE " + table_name + " SET " + columns + " WHERE " + condition + ";";
    return executeQuery(sql);
}


QueryResult DataBase::selectFromTable(const std::string& table_name, const std::string& columns, const std::string& condition) {
    std::string sql = "SELECT " + columns + " FROM " + table_name;
    if (!condition.empty()) {
        sql += " WHERE " + condition;
    }
    return executeQuery(sql);
}


void DataBase::rollBack(){
    sqlite3_exec(this->db, "ROLLBACK", NULL, NULL, NULL);
}


void DataBase::closeConnection(){
        sqlite3_close(this->db);    
}
