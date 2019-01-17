include_directories(../general/)
include_directories(../../riscv_sim/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

link_directories(../../riscv_sim/libs/)

file(GLOB src_all 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/region/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/connector/msg_transfer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/session/*.cpp"
)

add_executable(server main.cpp ${src_all})
target_link_libraries(server allGeneral riscv_sim)
