cmake_minimum_required(VERSION 2.9)
project(CW02)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -pedantic -Wextra")

set(SOURCE_FILES tree.c)
set(HEADER_FILES tree.h)
set(SOURCE_FILES2 tree_norm.c)
set(HEADER_FILES2 tree_norm.h)

add_executable(nftwprogram ${SOURCE_FILES})

add_executable(normprog ${SOURCE_FILES2} )

install(TARGETS nftwprogram DESTINATION ./zad2)
install(TARGETS normprogram DESTINATION ./zad2)
