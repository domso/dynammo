include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

include(FindPkgConfig)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

INCLUDE(FindPkgConfig)

find_package(OpenGL REQUIRED)

file(GLOB src_graphic "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/*.cpp")
add_library(graphicGameClient STATIC ${src_graphic})

file(GLOB src_user_interface "${CMAKE_CURRENT_SOURCE_DIR}/src/user_interface/*.cpp")
file(GLOB src_user_interface_views "${CMAKE_CURRENT_SOURCE_DIR}/src/user_interface/views/*.cpp")
add_library(user_interfaceGameClient STATIC ${src_user_interface} ${src_user_interface_views})
target_link_libraries(user_interfaceGameClient utilGeneral)

file(GLOB src_region "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp")
add_library(regionGameClient STATIC ${src_region})

file(GLOB src_session "${CMAKE_CURRENT_SOURCE_DIR}/src/session/*.cpp")
add_library(sessionGameClient STATIC ${src_session})
target_link_libraries(sessionGameClient encryptionGeneral)

file(GLOB src_connector "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/*.cpp")
file(GLOB src_connector_data_transfer "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/data_transfer/*.cpp")
file(GLOB src_connector_msg_transfer "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/msg_transfer/*.cpp")
add_library(connectorGameClient STATIC ${src_connector} ${src_connector_data_transfer} ${src_connector_msg_transfer})
target_link_libraries(connectorGameClient messageGeneral)


add_executable(gameClient main.cpp)
target_link_libraries(gameClient utilGeneral messageGeneral encryptionGeneral regionGameClient sessionGameClient graphicGameClient user_interfaceGameClient connectorGameClient)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(gameClient ${OPENGL_LIBRARIES} ${GTKMM_LIBRARIES} "epoxy")
