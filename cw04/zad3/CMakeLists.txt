cmake_minimum_required(VERSION 3.0)
project(cw04)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g")
set(CMAKE_C_STANDARD 11)

add_executable(program3 trzy.c)

install(TARGETS program3 DESTINATION ${CMAKE_SOURCE_DIR}/bin/zad3)
