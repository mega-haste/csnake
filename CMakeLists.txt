cmake_minimum_required(VERSION 3.20.0)
project(CSnake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE C_SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(CSnake main.c ${C_SRC_FILES})
target_include_directories(CSnake PUBLIC include/)
target_link_libraries(CSnake PUBLIC raylib)


