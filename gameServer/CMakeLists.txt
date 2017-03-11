cmake_minimum_required(VERSION 2.6)
include_directories(../network/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
project(dynammo)

add_executable(dynammo main.cpp src/service_map.cpp src/config_file_loader.cpp)

install(TARGETS dynammo RUNTIME DESTINATION bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++11 -fPIC -pthread")