include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

include(FindPkgConfig)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

INCLUDE(FindPkgConfig)

find_package(OpenGL REQUIRED)

file(GLOB src_region "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp")
add_library(regionGameClient STATIC ${src_region})

file(GLOB src_connector "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/*.cpp")
add_library(connectorGameClient STATIC ${src_connector})
target_link_libraries(connectorGameClient messageGeneral)

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
add_library(dataGameClient STATIC ${src_data})
target_link_libraries(dataGameClient regionGameClient)

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageGameClient STATIC ${src_message})

file(GLOB src_graphic "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/*.cpp")
add_library(graphicGameClient STATIC ${src_graphic})

add_executable(gameClient main.cpp)
target_link_libraries(gameClient utilGeneral messageGeneral encryptionGeneral regionGameClient graphicGameClient connectorGameClient dataGameClient messageGameClient)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(gameClient ${OPENGL_LIBRARIES} ${GTKMM_LIBRARIES} "epoxy")
