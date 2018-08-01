include_directories(/usr/include/openssl)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
find_package(OpenSSL REQUIRED)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_all 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/encryption/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/database/*.cpp"
)

add_library(allGeneral STATIC ${src_all})
target_link_libraries(allGeneral ${OPENSSL_LIBRARIES} mysqlclient)

