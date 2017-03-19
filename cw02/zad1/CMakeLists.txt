cmake_minimum_required(VERSION 2.9)
project(cw02)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -pedantic -Wextra")

set(SOURCE_FILES_1 generate.c)
set(SOURCE_FILES_2 sort.c)
set(HEADER_FILES sort.h)

add_executable(sort ${SOURCE_FILES_2})
add_executable(generate ${SOURCE_FILES_1})

install(TARGETS sort generate DESTINATION ${CMAKE_SOURCE_DIR}/bin/zad1)
