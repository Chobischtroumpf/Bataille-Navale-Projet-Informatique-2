#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../include/server/database/database.hh"
#include "../include/server/database/queries.hh"
#include "../include/server/database/query_result.hh"


#include <vector>
#include <string>


#define CUSTOM_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            std::cout << "Failed! " << std::endl; \
            CPPUNIT_ASSERT(expr); \
        } else { \
            std::cout << "Passed. " << std::endl; \
        } \
    } while (false)


class QueriesTest : public CppUnit::TestFixture {
    std::unique_ptr<Queries> queries;
    CPPUNIT_TEST_SUITE(QueriesTest);
    //CPPUNIT_TEST(testCreateDb);
    //CPPUNIT_TEST(testCreateTables);
    CPPUNIT_TEST(testValidUserRegister);
    CPPUNIT_TEST(testInvalidUserRegister);
    CPPUNIT_TEST(testValidCheckUserName);
    CPPUNIT_TEST(testInvalidCheckUserName);
    CPPUNIT_TEST(testValidCheckPwd);
    CPPUNIT_TEST(testInvalidCheckPwd);
    CPPUNIT_TEST(testValidSendMsg);
    CPPUNIT_TEST(testInvalidSendMsg);
    CPPUNIT_TEST(testValidAddFriend);
    CPPUNIT_TEST(testInvalidAddFriend);
    CPPUNIT_TEST(testValidUpdateUsername);
    CPPUNIT_TEST(testInvalidUpdateUsername);
    CPPUNIT_TEST(testValidDeleteFriend);
    CPPUNIT_TEST(testValidDeleteAccount);
    CPPUNIT_TEST(testInvalidDeleteAccount);
    CPPUNIT_TEST_SUITE_END();

public:

    void setUp() override {
        std::shared_ptr<DataBase> db = std::make_shared<DataBase>();
        queries = std::make_unique<Queries>(db);
    }

    void testValidUserRegister(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 1: testValidUserRegister" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"a", "azerty01"},
            {"b", "azerty02"},
            {"c", "azerty03"},
            {"d", "azerty04"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", " << user_data.second << "): ";
            bool  result = queries->userRegister(user_data.first, user_data.second);
            CUSTOM_ASSERT(result);
        }
    }

    void testInvalidUserRegister(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 1: testInvalidUserRegister" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> invalid_entry = {
            {"", "azerty01"},   // username empty
            {"b", "azerty02"},  // duplicate entry
        };
        for (const auto& user_data : invalid_entry) {
            std::cout << "(" << user_data.first << ", " << user_data.second << "): ";
            bool result = queries->userRegister(user_data.first, user_data.second);
            CUSTOM_ASSERT(!result);
        }   
    }

    void testValidCheckUserName(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 2: testValidCheckUserName" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::string> valid_entry = {
            {"a"},
            {"b"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data << "): ";
            QueryResult result = queries->checkUserName(user_data);
            CUSTOM_ASSERT(!result.data.empty());
            std::cout << "return: " << result << std::endl;
        }   
    }

    void testInvalidCheckUserName(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 2: testInvalidCheckUserName" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::string> invalid_entry = {
            {""},   // empty username
            {"r"},  // do not exists
        };
        for (const auto& user_data : invalid_entry) {
            std::cout <<"(" << user_data << "): ";
            QueryResult result = queries->checkUserName(user_data);
            CUSTOM_ASSERT(result.data.empty());
            std::cout << "return: " << result << std::endl;
        }    
    }

    void testValidCheckPwd(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 3: testValidCheckPwd" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"a", "azerty01"},
            {"b", "azerty02"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout <<"(" << user_data.first << ", " << user_data.second << "): ";
            QueryResult temp = queries->checkUserName(user_data.first);
            bool result = false;
            if(!temp.data.empty()){
                result = queries->checkPwd(temp.data[0][0], user_data.second);
            }
            CUSTOM_ASSERT(result);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testInvalidCheckPwd(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 3: testInvalidCheckPwd" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> invalid_entry = {
            {"a", "azerty000"},     // wrong password
            {"b", "azerty000"},     // wrong password
        };
        for (const auto& user_data : invalid_entry) {
            std::cout <<"(" << user_data.first << ", " << user_data.second << "): ";
            QueryResult temp = queries->checkUserName(user_data.first);
            bool result = false;
            if(!temp.data.empty()){
                result = queries->checkPwd(temp.data[0][0], user_data.second);
            }
            CUSTOM_ASSERT(!result);
            std::cout << "return: " << result << std::endl;
        } 
    }

    void testValidSendMsg(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 4: testValidSendMsg" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::tuple<std::string, std::string, std::string>> valid_entry = {
            {"1", "2", "Hello db, do you hear me ?"},
            {"2", "1", "Yeah"},
            {"3", "2", "Bonjour"},
            {"4", "2", "Hola"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << std::get<0>(user_data) << ", " << std::get<1>(user_data) << ", " << std::get<2>(user_data) << "): ";
            bool result = queries->sendMsg(std::get<0>(user_data), std::get<1>(user_data), std::get<2>(user_data));
            CUSTOM_ASSERT(result);
        }
    }

    void testInvalidSendMsg(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 4: testInvalidSendMsg" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::tuple<std::string, std::string, std::string>> invalid_entry = {
            {"1", "1", "Hello ?"},  // message himself
            {"2", "1", ""},         // empty message
            {"54", "1", "Dammed."}, // User_id do not exists.
        };
        for (const auto& user_data : invalid_entry) {
            std::cout << "(" << std::get<0>(user_data) << ", " << std::get<1>(user_data) << ", " << std::get<2>(user_data) << "): ";
            bool result = queries->sendMsg(std::get<0>(user_data), std::get<1>(user_data), std::get<2>(user_data));
            CUSTOM_ASSERT(!result);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testValidAddFriend(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 5: testValidAddFriend" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"1", "d"},
            {"1", "b"},
            {"2", "c"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", "  << user_data.second << "): ";
            bool result = queries->addFriend(user_data.first, user_data.second);
            CUSTOM_ASSERT(result);
        }
    }

    void testInvalidAddFriend(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 5: testInvalidAddFriend" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"", "b"},    // id do not exists
            {"54", "c"},    // id do not exists
            {"1", "a"},     // error friend with him self
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", "  << user_data.second << "): ";
            bool result = queries->addFriend(user_data.first, user_data.second);
            CUSTOM_ASSERT(!result);
        }
    }

    void testValidUpdateUsername(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 6: testValidUpdateUsername" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::tuple<std::string, std::string, std::string>> valid_entry = {
            {"1", "azerty01", "aa"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << std::get<0>(user_data) << ", " << std::get<1>(user_data) << ", " << std::get<2>(user_data) << "): ";
            QueryResult result = queries->updateUserName(std::get<0>(user_data), std::get<1>(user_data), std::get<2>(user_data));
            CUSTOM_ASSERT(result.error == DbError::OK);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testInvalidUpdateUsername(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 6: testInvalidUpdateUsername" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::tuple<std::string, std::string, std::string>> invalid_entry = {
            {"1", "azerty08", "aaa"},   // wrong pwd
            {"1", "azerty01", ""},      // empty username
            {"54", "1", "Dammed"},      // id do not exists.
        };
        for (const auto& user_data : invalid_entry) {
            std::cout << "(" << std::get<0>(user_data) << ", " << std::get<1>(user_data) << ", " << std::get<2>(user_data) << "): ";
            QueryResult result = queries->updateUserName(std::get<0>(user_data), std::get<1>(user_data), std::get<2>(user_data));
            CUSTOM_ASSERT(result.error != DbError::OK);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testValidDeleteFriend(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 9: testValidDeleteFriend" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"1", "2"},
            {"2", "3"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", "  << user_data.second << "): ";
            QueryResult result = queries->deleteFriend(user_data.first, user_data.second);
            CUSTOM_ASSERT(result.error == DbError::OK);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testValidDeleteAccount(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 10: testValidDeleteAccount" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"1", "azerty01"},
            {"2", "azerty02"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", "  << user_data.second << "): ";
            QueryResult result = queries->deleteAccount(user_data.first, user_data.second);
            CUSTOM_ASSERT(result.error == DbError::OK);
            std::cout << "return: " << result << std::endl;
        }
    }

    void testInvalidDeleteAccount(){
        std::cout << std::endl << "///////////////////////////////" << std::endl;
        std::cout << "Test 10: testInvalidDeleteAccount" << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::vector<std::pair<std::string, std::string>> valid_entry = {
            {"", "azerty01"},
            {"2", "azerty024"},
        };
        for (const auto& user_data : valid_entry) {
            std::cout << "(" << user_data.first << ", "  << user_data.second << "): ";
            QueryResult result = queries->deleteAccount(user_data.first, user_data.second);
            CUSTOM_ASSERT(result.error != DbError::OK);
            std::cout << "return: " << result << std::endl;
        }
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(QueriesTest);


int main() {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    bool result = runner.run("", false);
    return result ? 0 : 1;
}