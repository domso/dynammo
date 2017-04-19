include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/
)

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageGameClient STATIC ${src_message})

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
add_library(dataGameClient STATIC ${src_data})
target_link_libraries(dataGameClient messageGameClient)

add_executable(gameClient main.cpp)
target_link_libraries(gameClient utilGeneral encryptionGeneral messageGameClient dataGameClient)