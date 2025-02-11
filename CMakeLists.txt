cmake_minimum_required(VERSION 3.10)
project(Miner)

# Require C++17 or later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Folders to look for sources
SET (SOURCE_FOLDERS
    "logger"
    "net"
    "net/client"
    "net/master"
)

# Add all source files
set(SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)  # Initialize SOURCES with main.cpp
foreach(folder ${SOURCE_FOLDERS})
    file(GLOB FOLDER_SOURCES "${CMAKE_SOURCE_DIR}/src/${folder}/*.cpp")
    list(APPEND SOURCES ${FOLDER_SOURCES})
endforeach()

add_executable(mini_blockchain ${SOURCES})

# Link OpenSSL libraries
target_link_libraries(mini_blockchain OpenSSL::SSL OpenSSL::Crypto)
