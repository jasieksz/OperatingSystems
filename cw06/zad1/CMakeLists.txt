cmake_minimum_required(VERSION 3.0.2)
project(cw06)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD 11)

add_executable(server server.c server.h)
add_executable(client client.c)

install(TARGETS server DESTINATION ./zad1)
install(TARGETS client DESTINATION ./zad1)
