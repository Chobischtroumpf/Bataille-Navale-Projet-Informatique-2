#!/bin/bash

if [ ! -x "$(command -v jq)" ]; then
    echo "jq is not installed. Please install jq with 'sudo apt install jq' or 'brew install jq' and try again."
    exit 1
fi

username=$(./random_str.sh)
password=$(./random_str.sh)

username2="testuser"
password2="testpassword"

# Test 1: Register a new user
echo -e "\x1b[34mTEST 1: Register a new user... \x1b[0m"
response=$(curl -s -X POST -H "Content-Type: application/json" -d "{\"username\":\"$username\",\"password\":\"$password\"}" http://localhost:8080/api/register)

if echo $response | jq -e 'has("error")' > /dev/null; then
    echo -e "\e[1;31mTEST 1: Failed\e[0m \x1b[3m echoing response \x1b[0m"
    echo $response
    exit 1
else
    echo -e "\x1b[32mTEST 1: PASSED\x1b[0m"
    token=$(echo "$response" | jq -r '.authToken')
fi

# Test 2: Get userId of a user in a database
echo -e "\x1b[34mTEST 2: Get userId of a database member \x1b[0m"
response=$(curl -s -X GET http://localhost:8080/api/login/uid?username=$username)

if echo $response | jq -e 'has("error")' > /dev/null; then
    echo -e "\e[1;31mTEST 2: Failed\e[0m"
    echo -e "\x1b[3mEchoing response...\x1b[0m"
    echo $response
    exit 1
else
    echo -e "\x1b[32mTEST 2: PASSED\x1b[0m"
    userId=$(echo "$response" | jq -r '.userId')
fi

# Test 3: Send a message
echo -e "\x1b[34mTEST 3: Send a message to a user \x1b[0m"

response=$(curl -s -X POST -H "Content-Type: application/json" -H "Authorization: Bearer $token" -d "{\"message\":\"$userId\", \"recipientId\": \"romek\"}" http://localhost:8080/api/chat/send)

if echo $response | jq -e 'has("error")' > /dev/null; then
    echo -e "\e[1;31mTEST 3: Failed\e[0m"
    echo -e "\x1b[3mEchoing response...\x1b[0m"
    echo $response
    exit 1;
else
    echo -e "\x1b[32mTEST 3: PASSED\x1b[0m"
fi

# Test 4: Login with the new user
echo -e "\x1b[34mTEST 4: Connect to a user\x1b[0m"

response=$(curl -s -X POST -H "Content-Type: application/json" -d "{\"username\":\"$username2\",\"password\":\"$password2\"}" http://localhost:8080/api/login)

if echo $response | jq -e 'has("error")' > /dev/null; then
    echo -e "\e[1;31mTEST 4: Failed\e[0m"
    echo -e "\x1b[3mEchoing response...\x1b[0m"
    echo $response
    exit 1;
else
    echo -e "\x1b[32mTEST 4: PASSED\x1b[0m"
    token2=$(echo "$response" | jq -r '.authToken')
fi

#Test 5: Get messages
echo -e "\x1b[34mTEST 5: Get messages\x1b[0m"

response=$(curl -s -X GET -H "Content-Type: application/json" -H "Authorization: Bearer $token2$" -d "{\"userId\": \"$userId\"}" http://localhost:8080/api/chat/get)

if echo $response | jq -e 'has("error")' > /dev/null; then
    echo -e "\e[1;31mTEST 5: Failed\e[0m"
    echo -e "\x1b[3mEchoing response...\x1b[0m"
    echo $response
    exit 1;
else
    echo -e "\x1b[32mTEST 5: PASSED\x1b[0m"
fi