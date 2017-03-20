cmake_minimum_required(VERSION 2.9)
project(CW02)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -pedantic -Wextra")

set(SOURCE_FILES tree.c)
set(HEADER_FILES tree.h)

add_executable(dirprogram ${SOURCE_FILES})

install(TARGETS dirprogram DESTINATION ${CMAKE_SOURCE_DIR}/bin/zad2)
