cmake_minimum_required(VERSION 3.20)
project(My_BigNum_GF_cal C)

set(CMAKE_C_STANDARD 99)

add_library(My_BigNum_GF_cal SHARED library.c)
target_link_libraries(My_BigNum_GF_cal -lgmp)
target_link_libraries(My_BigNum_GF_cal -lm)