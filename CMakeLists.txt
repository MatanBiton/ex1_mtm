cmake_minimum_required(VERSION 3.23)
project(ex1_mtm C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex1_mtm
        IsraeliQueue.c
        IsraeliQueue.h)
