cmake_minimum_required(VERSION 3.13)
project(CP_Lab_1)

set(CMAKE_CXX_STANDARD 14)

set(LINKED_LIST
        linked_list.cpp
        linked_list.h
        )

add_executable(CP_Lab_1 main.cpp ${LINKED_LIST})