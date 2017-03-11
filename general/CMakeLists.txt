cmake_minimum_required(VERSION 3.5)

include_directories(/usr/include/openssl)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
find_package(OpenSSL REQUIRED)

set (
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/
)

file(GLOB src_util "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp")
add_library(util STATIC ${src_util})

file(GLOB src_encryption "${CMAKE_CURRENT_SOURCE_DIR}/src/encryption/*.cpp")
add_library(encryption STATIC ${src_encryption})
target_link_libraries(encryption ${OPENSSL_LIBRARIES})

file(GLOB src_database "${CMAKE_CURRENT_SOURCE_DIR}/src/database/*.cpp")
add_library(database STATIC ${src_database})
target_link_libraries(database mysqlclient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS}  -std=c++11 -fPIC -pthread")

