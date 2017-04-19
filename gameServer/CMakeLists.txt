include_directories(../general/)
include_directories(../loginClient/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageGameServer STATIC ${src_message})

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
add_library(dataGameServer STATIC ${src_data})
target_link_libraries(dataGameServer utilGeneral encryptionGeneral messageGameServer)

add_executable(gameServer main.cpp)
target_link_libraries(gameServer utilGeneral encryptionGeneral dataGameServer messageGameServer)