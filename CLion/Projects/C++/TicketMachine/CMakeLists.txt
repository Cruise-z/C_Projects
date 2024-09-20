cmake_minimum_required(VERSION 3.27)
project(TicketMachine)

set(CMAKE_CXX_STANDARD 17)

add_executable(TicketMachine main.cpp
        TicketMachine.cpp
        TicketMachine.h)
