cmake_minimum_required(VERSION 3.0.2)
project(CW03)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD C11)

add_executable(master master.c)
add_executable(slave slave.c)

target_link_libraries(slave m)
target_link_libraries(master m)

install(TARGETS master DESTINATION ./zad2)
install(TARGETS slave DESTINATION ./zad2)
