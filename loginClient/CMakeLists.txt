include_directories(../general/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/
)

file(GLOB src_data "${CMAKE_CURRENT_SOURCE_DIR}/src/data/*.cpp")
add_library(dataLoginClient STATIC ${src_data})

file(GLOB src_message "${CMAKE_CURRENT_SOURCE_DIR}/src/message/*.cpp")
add_library(messageLoginClient STATIC ${src_message})

file(GLOB src_loginClient "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(LoginClient STATIC ${src_loginClient})
target_link_libraries(LoginClient dataLoginClient messageLoginClient)