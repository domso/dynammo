cmake_minimum_required(VERSION 3.5)

project(dynammo)
include_directories(../network/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -O2 -std=c++11 -fPIC -pthread")

add_subdirectory(general/)
add_subdirectory(loginServer/)
add_subdirectory(gameClient)
add_subdirectory(gameServer/)
