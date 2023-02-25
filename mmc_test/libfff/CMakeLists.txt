
cmake_minimum_required(VERSION 3.7.2)

project(libfff C)

add_library(lib_fff src/fff.c)
target_include_directories(lib_fff PUBLIC include/)

