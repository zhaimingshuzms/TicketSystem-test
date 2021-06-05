cmake_minimum_required(VERSION 3.16.3)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp)

add_compile_options(-Ofast)