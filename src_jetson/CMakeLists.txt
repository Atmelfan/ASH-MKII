cmake_minimum_required(VERSION 3.7)
project(src_jetson)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp cores/core.cpp cores/core.h)
add_executable(src_jetson ${SOURCE_FILES})