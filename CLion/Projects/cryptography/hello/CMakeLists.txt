cmake_minimum_required(VERSION 3.20)
project(hello C)

set(CMAKE_C_STANDARD 99)

add_library(hello SHARED library.c)
