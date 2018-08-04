include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

include(FindPkgConfig)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

INCLUDE(FindPkgConfig)

find_package(OpenGL REQUIRED)

file(GLOB src_all 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/*.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/mesh/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/shader/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graphic/texture/*.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/user_interface/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/user_interface/views/*.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/mesh/*.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/data_transfer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/msg_transfer/*.cpp"
)

add_executable(client main.cpp ${src_all})
target_link_libraries(client allGeneral)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(client ${OPENGL_LIBRARIES} ${GTKMM_LIBRARIES} "epoxy")
