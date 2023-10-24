cmake_minimum_required(VERSION 3.21)
project(RootsLib)

set(CMAKE_CXX_STANDARD 20)


add_library(RootsLib roots_library/library.cpp)

target_include_directories(RootsLib PUBLIC /Users/alex/Library/boost/boost_1_79_0)
target_link_directories(RootsLib PUBLIC /Users/alex/Library/boost/boost_1_79_0/stage/lib)

#target_link_libraries(RootsLib /Users/alex/Library/boost/boost_1_79_0/stage/lib)

add_subdirectory(Google_tests)

