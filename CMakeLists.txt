cmake_minimum_required(VERSION 3.10) # minimum version of CMake
cmake_policy(SET CMP0135 NEW) # Silence the warning for FetchContent

project(Battleship) # The project name

# Add fetch content macro to fetch external libraries from their repo
include(FetchContent)

# Fetch the nlohmann json library
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(include)

function(get_sources dir result)
    file(GLOB_RECURSE sources "${dir}/*.cpp" "${dir}/*.cc")
    set(${result} ${sources} PARENT_SCOPE)
endfunction()

# Client sources
get_sources("src/client" CLIENT_SOURCES)

# Server sources
get_sources("src/server" SERVER_SOURCES)

# Common sources
get_sources("src/common" COMMON_SOURCES)

# Create client executable
add_executable(battleship ${CLIENT_SOURCES} ${COMMON_SOURCES})

# Create server executable
add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES})

# Define an executable target built from `client.cpp` and its dependencies
add_executable(client 
src/client/client.cpp 
src/client/network/GameClient.cpp
)

# Define include directories for target 'client' 
target_include_directories(client PRIVATE include/client/network)

# Define include directories for target 'server'
target_include_directories(server PRIVATE include/server/database include/server/game include/server/network)

# Specifies libraries for targets
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)

# using libraries like cpprestsdk, link with both executables
find_package(cpprestsdk REQUIRED)

if(NOT cpprestsdk_FOUND)
  message(WARNING "Cpprestsdk not found. Please run the script provided under linux/installCppRestSDK.sh to install cpprestsdk. Admin privileges are likely required")
endif()

target_link_libraries(server PRIVATE cpprestsdk::cpprest crypto)
target_link_libraries(client PRIVATE cpprestsdk::cpprest crypto)
