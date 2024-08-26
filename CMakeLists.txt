cmake_minimum_required(VERSION 3.30.2)
project(clox)

add_library(compilerLib compiler.c compiler.h)
add_library(debugLib debug.c debug.h)
add_library(chunkLib chunk.c chunk.h)
add_library(memoryLib memory.c memory.h)
add_library(scannerLib scanner.c scanner.h)
add_library(valueLib value.c value.h)
add_library(vmLib vm.c vm.h)

add_executable(clox main.c)

target_link_libraries(clox PRIVATE compilerLib debugLib chunkLib memoryLib scannerLib valueLib vmLib)