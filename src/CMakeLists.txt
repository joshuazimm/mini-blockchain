cmake_minimum_required(VERSION 3.10)
project(Miner)

# Require C++17 or later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(mini_blockchain main.cpp)

# Link OpenSSL libraries
target_link_libraries(mini_blockchain OpenSSL::SSL OpenSSL::Crypto)
