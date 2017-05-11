cmake_minimum_required(VERSION 3.3)
project(tsp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES src/main.c src/printer.c include/printer.h src/algorithms.c include/algorithms.h include/structures.h)
add_executable(tsp ${SOURCE_FILES})