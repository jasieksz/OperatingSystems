cmake_minimum_required(VERSION 3.6)
project(bst)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c bst.c)
add_executable(bst ${SOURCE_FILES})