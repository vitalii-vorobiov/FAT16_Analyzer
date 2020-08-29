cmake_minimum_required(VERSION 3.17)
project(fat16)

set(CMAKE_CXX_STANDARD 17)

add_executable(fat16 main.cpp include/bootsector.h include/file.h)