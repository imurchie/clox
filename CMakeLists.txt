cmake_minimum_required(VERSION 3.30.2)
project(clox)

add_library(debugLib debug.c debug.h)
add_library(chunkLib chunk.c chunk.h)
add_library(memoryLib memory.c memory.h)
add_library(valueLib value.c value.h)

add_executable(clox main.c)

target_link_libraries(clox PRIVATE debugLib chunkLib memoryLib valueLib)