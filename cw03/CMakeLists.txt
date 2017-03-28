cmake_minimum_required (VERSION 2.9)
project (CW03)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD C11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(zad1)
add_subdirectory(zad2)
