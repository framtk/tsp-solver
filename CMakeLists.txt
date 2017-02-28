cmake_minimum_required(VERSION 3.3)
project(tsp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c printer.c printer.h algorithms.c algorithms.h)
add_executable(tsp ${SOURCE_FILES})