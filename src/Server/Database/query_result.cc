#include "query_result.hh"


std::string QueryResult::getFirst(){
    std::string res;
    try{res = data[0][0];
        }catch (...) {
        res = "";
    }
    return res;
}

bool QueryResult::isOk(){
    return error == DbError::OK;
}

std::string QueryResult::getError(){
    std::string err;
    switch(error) {
        case DbError::OK:
            err = "No issues.";
            break;
        case DbError::EXECUTION_ERROR:
            err = "Unknow error.";
            break;
        case DbError::NON_EXISTENT_USER_NAME:
            err = "The username do not exists.";
            break;
        case DbError::NON_EXISTENT_USER_ID:
            err = "The user id do not exists.";
            break;
        case DbError::WRONG_PWD_FAILED:
            err = "Wrong password.";
            break;
        case DbError::UNIQUE_CONSTRAINT_FAILED:
            err = "Entry already exists.";
            break;
        case DbError::CHECK_USERNAME_CONSTRAINT_FAILED:
            err = "Empty username.";
            break;
        case DbError::CHECK_RELATION_CONSTRAINT_FAILED:
            err = "User can not be friend with himself.";
            break;
        case DbError::CHECK_MSG_CONSTRAINT_FAILED:
            err = "Message can not be empty.";
            break;
        case DbError::CANNOT_UPDATE_ID_COLUMN:
            err = "Can not update an id column.";
            break;
        case DbError::CHECK_SENDER_CONSTRAINT_FAILED:
            err = "User can not send a message to himself.";
            break;
        case DbError::FOREIGN_KEY_CONSTRAINT_FAILED:
            err = "ID do not exists.";
            break;             
        default:
            err = "Unrecognized error.";
            break;
    }
    return err;
}

// USED IN TEST ONLY
std::ostream& operator<<(std::ostream& os, const DbError& dberror){
    switch(dberror) {
        case DbError::OK:
            os << "No issues.";
            break;
        case DbError::EXECUTION_ERROR:
            os << "Unknow error.";
            break;
        case DbError::NON_EXISTENT_USER_NAME:
            os << "The username do not exists.";
            break;
        case DbError::WRONG_PWD_FAILED:
            os << "Wrong password.";
            break;
        case DbError::UNIQUE_CONSTRAINT_FAILED:
            os << "Entry already exists.";
            break;
        case DbError::CHECK_USERNAME_CONSTRAINT_FAILED:
            os << "Empty username.";
            break;
        case DbError::CHECK_RELATION_CONSTRAINT_FAILED:
            os << "User can not be friend with himself.";
            break;
        case DbError::CHECK_MSG_CONSTRAINT_FAILED:
            os << "Message can not be empty.";
            break;
        case DbError::CANNOT_UPDATE_ID_COLUMN:
            os << "Can not update an id column.";
            break;
        case DbError::CHECK_SENDER_CONSTRAINT_FAILED:
            os << "User can not send a message to himself.";
            break;
        case DbError::FOREIGN_KEY_CONSTRAINT_FAILED:
            os << "ID do not exists.";
            break;             
        default:
            os << "Unrecognized error.";
            break;
    }
    return os;
}

// USED IN TEST ONLY
std::ostream& operator<<(std::ostream& os, const QueryResult& result){
    os << "QueryResult: { ";
    os << "Data: [ ";
    for(const auto& row : result.data) {
        for(const auto& item : row) {
            os << item << " ";
        }
    }
    os << "], Error: " << result.error;
    os << " }";
    return os;
}
