include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_message_account "${CMAKE_CURRENT_SOURCE_DIR}/src/message/account/*.cpp")
file(GLOB src_message_game "${CMAKE_CURRENT_SOURCE_DIR}/src/message/game/*.cpp")
add_library(messageGameClient STATIC ${src_message_account} ${src_message_game})

file(GLOB src_controller_account "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/account/*.cpp")
add_library(controllerGameClient STATIC ${src_controller_account})
target_link_libraries(controllerGameClient messageGameClient)

add_executable(gameClient main.cpp)
target_link_libraries(gameClient utilGeneral encryptionGeneral messageGameClient controllerGameClient)
