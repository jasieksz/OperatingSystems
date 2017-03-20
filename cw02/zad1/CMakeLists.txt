cmake_minimum_required(VERSION 2.9)
project(CW02)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -pedantic -Wextra")

set(SOURCE_FILES sort_shuffle.c)
set(HEADER_FILES sort_shuffle.h)

add_executable(program ${SOURCE_FILES})

install(TARGETS program DESTINATION ./zad1)
