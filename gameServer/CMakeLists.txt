include_directories(../general/)
include_directories(../loginClient/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_region "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp")
add_library(regionGameServer STATIC ${src_region})
target_link_libraries(regionGameServer utilGeneral messageGeneral)

file(GLOB src_connection "${CMAKE_CURRENT_SOURCE_DIR}/src/connection/*.cpp")
add_library(connectionGameServer STATIC ${src_connection})
target_link_libraries(connectionGameServer utilGeneral messageGeneral)

file(GLOB src_connector "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/*.cpp")
file(GLOB src_connector_msg_transfer "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/msg_transfer/*.cpp")
add_library(connectorGameServer STATIC ${src_connector} ${src_connector_msg_transfer})
target_link_libraries(connectorGameServer utilGeneral messageGeneral)

file(GLOB src_user "${CMAKE_CURRENT_SOURCE_DIR}/src/user/*.cpp")
add_library(userGameServer STATIC ${src_user})
target_link_libraries(connectorGameServer utilGeneral messageGeneral)

file(GLOB src_action "${CMAKE_CURRENT_SOURCE_DIR}/src/action/*.cpp")
add_library(actionGameServer STATIC ${src_action})
# target_link_libraries(connectorGameServer utilGeneral messageGeneral)

#file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
#add_library(messageGameServer STATIC ${src_message} )

add_executable(gameServer main.cpp)
target_link_libraries(gameServer utilGeneral messageGeneral regionGameServer connectionGameServer connectorGameServer userGameServer actionGameServer encryptionGeneral ) #messageGameServer
