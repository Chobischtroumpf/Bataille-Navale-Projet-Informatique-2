#include "token_handler.hh"
#include <cassert>

void testTokenHandler() {
    // Test case 1: Verify token generation
    TokenHandler tokenHandler;
    std::string token = tokenHandler.generateToken(1);
    assert(!token.empty());

    // Test case 2: Verify token validation
    bool isValid = tokenHandler.validateToken(0,token);
    assert(isValid);

    // Test case 3: Verify invalid token validation
    isValid = tokenHandler.validateToken(0,"invalid_token");
    assert(!isValid);
}

int main() {
    testTokenHandler();
    return 0;

}