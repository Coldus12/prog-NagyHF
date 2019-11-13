cmake_minimum_required(VERSION 3.15)
project(testing C)

set(CMAKE_C_STANDARD 99)

add_library(testing library.c library.h)