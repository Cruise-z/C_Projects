cmake_minimum_required(VERSION 3.24)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test
        test.c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")