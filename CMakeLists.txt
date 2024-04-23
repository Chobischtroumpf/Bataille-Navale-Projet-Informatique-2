cmake_minimum_required(VERSION 3.16) # minimum version of CMake


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug build.")
  add_compile_options(-g -pedantic -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DOUTPUT_DEBUG=1)
  add_link_options(-fsanitize=address,undefined)
else(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Release build.")
  add_compile_options(-O3 -w)
endif()

# Specify the C++ compiler
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
set(CMAKE_CXX_COMPILER g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add compile options
add_compile_options(-std=c++20)
project(Battleship) # The project name



# Add fetch content macro to fetch external libraries from their repo
include(FetchContent)


# if the path local path to lib/usr/lib/x86_64-linux-gnu exists, assumes the libraries are manually installed
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu/)
  include(${CMAKE_CURRENT_SOURCE_DIR}/lib/sodium.cmake)
else()
  # Add the libraries installation prefix to CMAKE_PREFIX_PATH
  list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu")
  list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu/cmake")
  
  # Find the sodium package
  set(sodium_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib)
  set(sodium_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/include/sodium/)
  set(sodium_LIBRARY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu/libsodium.a)
  set(sodium_LIBRARY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu/libsodium.a)
  
  include(${CMAKE_CURRENT_SOURCE_DIR}/lib/sodium.cmake)

  set(boost_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/lib/x86_64-linux-gnu)
  set(boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/include/boost)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/usr/include/x86_64-linux-gnu)
endif()

# Find the SQLite3 package
find_package(SQLite3 REQUIRED)



# Find the Boost package
# find_package(Boost REQUIRED)

# Fetch the nlohmann json library
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

if(POLICY CMP0100)
cmake_policy(SET CMP0100 NEW)
endif()

set(CMAKE_AUTOMOC ON)

# Add the include directories
include_directories(include)

function(get_sources dir result)
    file(GLOB_RECURSE sources 
    "${dir}/*.cc"
    "${dir}/*.hh")
    set(${result} ${sources} PARENT_SCOPE)
endfunction()

# function(get_sources_test dir result)
#     file(GLOB_RECURSE sources "${dir}/*.cc")
#     # remove server.cc from the test sources
#     list(FILTER sources EXCLUDE REGEX ".*server.cc")
#     set(${result} ${sources} PARENT_SCOPE)
# endfunction()

# Client sources
# Récupère les fichiers source
get_sources("src/Client" CLIENT_SOURCES)

# Récupère également les fichiers d'en-tête
get_sources("include/Client" CLIENT_HEADERS)



# Server sources
get_sources("src/Server" SERVER_SOURCES)

# # Server sources
# get_sources_test("src/Server" TEST_SOURCES)

# Common sources
get_sources("src/Common" COMMON_SOURCES)

# Common headers
get_sources("include/Common" COMMON_HEADERS)

# Create client executable
add_executable(battleship ${COMMON_SOURCES} ${CLIENT_SOURCES} ${CLIENT_HEADERS} ${COMMON_HEADERS})

set(CMAKE_AUTOMOC OFF)

# Create server executable
add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES})


#create test executable
# add_executable(tests test/test_game.cc ${COMMON_SOURCES} ${TEST_SOURCES})

# Define include directories for target 'client' 
# target_include_directories(client PRIVATE include/Client/Network)

# Define include directories for target 'server'
target_include_directories(server PRIVATE include/Server include/Server/Database include/Server/Game include/Server/Network include/Common include/Common/Commandant/Abilities include/Common/Commandant/Factions)

# Define include directories for target 'battleship'
target_include_directories(battleship PRIVATE  include/Client include/Client/Network include/Client/Controllers include/Client/Views include/Client/Display include/Client/Display/Console include/Client/Display/GUI include/Common include/Common/Commandant/Abilities include/Common/Commandant/Factions)

# Define include directories for target 'tests'
# target_include_directories(tests PRIVATE include/Server include/Server/Database include/Server/Game include/Server/Network include/Common include/Common/Commandant/Abilities include/Common/Commandant/Factions)

# Specifies libraries for targets
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(battleship PRIVATE nlohmann_json::nlohmann_json)
# target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

# using libraries like cpprestsdk, link with both executables
find_package(cpprestsdk REQUIRED)

if(NOT cpprestsdk_FOUND)
  message(WARNING "Cpprestsdk not found. Please run the script provided under linux/installCppRestSDK.sh to install cpprestsdk. Admin privileges are likely required")
endif()

# find qt
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(${Qt6_INCLUDE_DIRS})


set(CMAKE_AUTORCC ON) #*.qrc
set(CMAKE_AUTOUIC ON) #*.ui

target_link_libraries(server PRIVATE cpprestsdk::cpprest crypto)
target_link_libraries(battleship PRIVATE cpprestsdk::cpprest crypto)
target_link_libraries(server PRIVATE SQLite::SQLite3)
target_link_libraries(server PRIVATE sodium)
target_link_libraries(battleship PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
