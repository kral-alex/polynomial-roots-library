
project(Google_tests)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run RootsTest.cpp)

#[[target_include_directories(Google_Tests_run PUBLIC /Users/alex/Library/boost/boost_1_79_0)
target_link_directories(Google_Tests_run PUBLIC /Users/alex/Library/boost/boost_1_79_0/stage/lib)
target_link_libraries(Google_Tests_run /Users/alex/Library/boost/boost_1_79_0/stage/lib)]]

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run RootsLib)

target_link_libraries(Google_Tests_run gtest gtest_main)


