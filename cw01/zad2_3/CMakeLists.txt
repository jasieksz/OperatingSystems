cmake_minimum_required (VERSION 2.9)
project (CW01)

option(OPTIMIZATION "Optimization level" -o)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -o${OPTIMIZATION} -Wall -pedantic -Wextra")
set(CMAKE_C_STANDARD C11)

add_executable(test_static main.c)
add_executable(test_shared main.c)
add_executable(test_dynamic main_dynamic.c)

target_link_libraries(test_dynamic ${CMAKE_DL_LIBS})
target_link_libraries(test_shared zad1_shared_lib)
target_link_libraries(test_static zad1_static_lib)

install(TARGETS test_dynamic DESTINATION .)
install(TARGETS test_static DESTINATION .)
install(TARGETS test_shared DESTINATION .)
