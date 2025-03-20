cmake_minimum_required(VERSION 3.21)

project(Google_tests)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run RootsTest.cpp)

target_link_libraries(Google_Tests_run RootsLib)

target_link_libraries(Google_Tests_run gtest gtest_main)


