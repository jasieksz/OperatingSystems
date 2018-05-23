cmake_minimum_required(VERSION 3.0.2)
project(cw07)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall -pthread")
set(CMAKE_C_STANDARD 11)

add_executable(program main.c fft.h)

target_link_libraries(program rt)

install(TARGETS program DESTINATION ./zad1)