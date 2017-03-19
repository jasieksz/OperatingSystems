cmake_minimum_required (VERSION 2.9)
project (CW01)

option(OPTIMIZATION "Optimization level" -o)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -o${OPTIMIZATION} -Wall -pedantic -Wextra")
set(CMAKE_C_STANDARD C11)


set(SOURCE_FILES person.c list.c tree.c)
set(HEADER_FILES person.h list.h tree.h)

add_library(zad1_static_lib STATIC ${SOURCE_FILES})
add_library(zad1_shared_lib SHARED ${SOURCE_FILES})

install(TARGETS zad1_static_lib DESTINATION .)
install(TARGETS zad1_shared_lib DESTINATION .)