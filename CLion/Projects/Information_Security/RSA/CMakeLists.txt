cmake_minimum_required(VERSION 3.20)
project(RSA C)

set(CMAKE_C_STANDARD 99)

add_executable(RSA main.c)

target_link_libraries(RSA -lgmp)
target_link_libraries(RSA -lm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX FLAGS -fexec-charset=GBK)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#修改clion 内存
MATH(EXPR stack_size "100000000")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
