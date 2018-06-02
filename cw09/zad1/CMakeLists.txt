cmake_minimum_required(VERSION 3.0.2)
project(cw09)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall -pthread -lm")
set(CMAKE_C_STANDARD 11)

add_executable(program main.c bounded.h)

target_link_libraries(program rt)

install(TARGETS program DESTINATION ./zad1)
