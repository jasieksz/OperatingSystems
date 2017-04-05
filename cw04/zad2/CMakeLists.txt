cmake_minimum_required(VERSION 3.0)
project(cw04)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g")
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES dwa.c)
add_executable(program2 ${SOURCE_FILES})

install(TARGETS program2 DESTINATION ${CMAKE_SOURCE_DIR}/bin/zad2)
