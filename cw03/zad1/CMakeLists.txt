cmake_minimum_required(VERSION 2.9)
project(CW03)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")

set(SOURCE_FILES interpreter.c)
set(HEADER_FILES interpreter.h)

add_executable(program ${SOURCE_FILES})

install(TARGETS program DESTINATION ./zad1)
