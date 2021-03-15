cmake_minimum_required(VERSION 3.17)
project(Library C)

set(CMAKE_C_STANDARD 99)

add_executable(Library main.c library.h library.c)