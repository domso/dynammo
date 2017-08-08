include_directories(../general/)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_database "${CMAKE_CURRENT_SOURCE_DIR}/src/database/queries/*.cpp")
add_library(databaseLoginServer STATIC ${src_database})
target_link_libraries(databaseLoginServer utilGeneral databaseGeneral)

file(GLOB src_controller "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/*.cpp")
add_library(controllerLoginServer STATIC ${src_controller})
target_link_libraries(controllerLoginServer databaseLoginServer)

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageLoginServer STATIC ${src_message})

add_executable(loginServer main.cpp)
target_link_libraries(loginServer utilGeneral encryptionGeneral databaseGeneral databaseLoginServer controllerLoginServer messageLoginServer)
