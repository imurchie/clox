cmake_minimum_required(VERSION 3.30.2)
project(clox)

add_library(chunk chunk.c chunk.h)
add_library(memory memory.c memory.h)

add_executable(clox main.c)

target_link_libraries(clox PRIVATE chunk memory)