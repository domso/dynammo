cmake_minimum_required(VERSION 3.5)

project(dynammo)
include_directories(../network/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -Wall -g -std=c++17 -pthread")

add_subdirectory(general/)
add_subdirectory(gameClient)
add_subdirectory(gameServer/)
