cmake_minimum_required(VERSION 2.9)
project(CW03)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES interpreter_limit.c)
set(HEADER_FILES interpreter_limit.h)

add_executable(program_limit ${SOURCE_FILES})
add_executable(test_limit test2.c)

install(TARGETS program_limit DESTINATION ./zad2)
install(TARGETS test_limit DESTINATION ./zad2)
