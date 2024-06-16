cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 20)

project(practice
    VERSION 1.0)



add_executable(practice main.cpp Client.cpp worker.cpp)