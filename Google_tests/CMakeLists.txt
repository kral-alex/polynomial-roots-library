
project(Google_tests)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run HelloTest.cpp RootsTest.cpp)

target_compile_options(Google_Tests_run PUBLIC -lgtest_main  -lgtest)

# linking Google_Tests_run with lib which will be tested
target_link_libraries(Google_Tests_run RootsLib)

target_link_libraries(Google_Tests_run gtest)
target_link_libraries(Google_Tests_run gtest_main)
# target_link_libraries(Google_Tests_run gtest gtest_main PRIVATE -lgtest_main -lgtest)

