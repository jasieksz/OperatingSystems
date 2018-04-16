cmake_minimum_required(VERSION 3.0.2)
project(CW03)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES pipes.c)

add_executable(pipes ${SOURCE_FILES})

install(TARGETS pipes DESTINATION ./zad1)
