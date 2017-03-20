cmake_minimum_required(VERSION 2.9)
project(CW02)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -o2 -Wall -pedantic -Wextra")

set(SOURCE_FILES lock.c)
set(HEADER_FILES lock.h)


add_executable(lockprogram ${SOURCE_FILES})

install(TARGETS lockprogram DESTINATION ./zad3)
