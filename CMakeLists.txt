cmake_minimum_required(VERSION 3.21)
project(RootsLib)

set(CMAKE_CXX_STANDARD 20)



add_library(RootsLib roots_library/library.cpp)

add_subdirectory(Google_tests)