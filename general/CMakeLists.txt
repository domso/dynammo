include_directories(/usr/include/openssl)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
find_package(OpenSSL REQUIRED)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_util "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp")
add_library(utilGeneral STATIC ${src_util})

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageGeneral STATIC ${src_message})

file(GLOB src_encryption "${CMAKE_CURRENT_SOURCE_DIR}/src/encryption/*.cpp")
add_library(encryptionGeneral STATIC ${src_encryption})
target_link_libraries(encryptionGeneral ${OPENSSL_LIBRARIES})

file(GLOB src_database "${CMAKE_CURRENT_SOURCE_DIR}/src/database/*.cpp")
add_library(databaseGeneral STATIC ${src_database})
target_link_libraries(databaseGeneral mysqlclient)

#file(GLOB src_region "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp")
#add_library(regionGeneral STATIC ${src_region})

