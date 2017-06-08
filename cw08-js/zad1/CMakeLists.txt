cmake_minimum_required(VERSION 2.9)
project(cw08)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")

add_executable(generator generator.c)
add_executable(programA prog_a.c)
add_executable(programB prog_b.c)
# add_executable(programC prog_c.c)

target_link_libraries(programA pthread)
target_link_libraries(programB pthread)
# target_link_libraries(programC pthread)

install(TARGETS programA DESTINATION ./zad1)
install(TARGETS programB DESTINATION ./zad1)
# install(TARGETS programC DESTINATION ./zad1)
install(TARGETS generator DESTINATION ./zad1)
