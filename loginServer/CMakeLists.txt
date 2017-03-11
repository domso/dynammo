cmake_minimum_required(VERSION 3.5)

include_directories(../general/)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_database "${CMAKE_CURRENT_SOURCE_DIR}/src/database/queries/*.cpp")
add_library(databaseLoginServer STATIC ${src_database})
target_link_libraries(databaseLoginServer util database)

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
add_library(dataLoginServer STATIC ${src_data})
target_link_libraries(dataLoginServer databaseLoginServer)

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageLoginServer STATIC ${src_message})

add_executable(loginServer main.cpp)
target_link_libraries(loginServer util encryption database databaseLoginServer dataLoginServer messageLoginServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS}  -std=c++11 -fPIC -pthread")

