include_directories(../general/)
include_directories(../loginClient/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_message_account "${CMAKE_CURRENT_SOURCE_DIR}/src/message/account/*.cpp")
file(GLOB src_message_game "${CMAKE_CURRENT_SOURCE_DIR}/src/message/game/*.cpp")
add_library(messageGameServer STATIC ${src_message_account} ${src_message_game})

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
file(GLOB src_data_map "${CMAKE_CURRENT_SOURCE_DIR}/src/data/map/*.cpp")
add_library(dataGameServer STATIC ${src_data} ${src_data_map})
target_link_libraries(dataGameServer utilGeneral encryptionGeneral messageGameServer)

file(GLOB src_controller_account "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/account/*.cpp")
file(GLOB src_controller_game "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/game/*.cpp")
add_library(controllerGameServer STATIC ${src_controller_account} ${src_controller_game})
target_link_libraries(controllerGameServer messageGameServer)

add_executable(gameServer main.cpp)
target_link_libraries(gameServer utilGeneral encryptionGeneral dataGameServer messageGameServer controllerGameServer)
