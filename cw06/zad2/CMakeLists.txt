cmake_minimum_required(VERSION 3.0.2)
project(cw06)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall")
set(CMAKE_C_STANDARD C11)

add_executable(server2 server2.c server2.h)
add_executable(client2 client2.c)

target_link_libraries(server2 rt)
target_link_libraries(client2 rt)

install(TARGETS server2 DESTINATION ./zad2)
install(TARGETS client2 DESTINATION ./zad2)
