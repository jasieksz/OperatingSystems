cmake_minimum_required(VERSION 3.0)
project(cw04)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wpedantic -Wextra")
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES printer.c)
add_executable(printer ${SOURCE_FILES})

install(TARGETS printer DESTINATION ${CMAKE_SOURCE_DIR}/bin/zad1)
