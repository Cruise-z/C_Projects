cmake_minimum_required(VERSION 3.20)
project(test C)

set(CMAKE_C_STANDARD 99)

add_executable(test main.c)
target_link_libraries(test -lm)
target_link_libraries(test -lgmp)