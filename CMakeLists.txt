set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
cmake_minimum_required(VERSION 3.14)

# set the project name
project(Bit-Vector VERSION 0.1)

# set output directory of builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_BENCHMARKS "Build Google Bechmarks" OFF)

# specify global compiler flags
add_compile_options(-std=c++20 -march=native -pedantic -Wall -Wextra -Wfatal-errors)

# add the tests executable
file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(tests ${TEST_SOURCES})

# configure benchmarks
#if (BUILD_BENCHMARKS)
    #add_executable(benchmarks src/benchmarks/benchmark_main.cc)
    #target_compile_options(benchmarks PUBLIC -O3 -DNDEBUG -mtune=native)
    #find_package(benchmark)
    #target_link_libraries(benchmarks benchmark::benchmark)
    #target_include_directories(benchmarks PRIVATE ./ext/libsimdpp ./include ./ext/libsimdpp ./ext/bit)
    #install(TARGETS benchmarks DESTINATION .)
#endif()

# specify the C++ standard
set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED true)

# specify global include directories
include_directories(./include ./ext/bit/cpp ./ext/bit-algorithms/include)

# specify test-specific flags
target_compile_options(tests PUBLIC -O0 -g -fno-omit-frame-pointer -pg)

# specify test-specific libraries
find_library(GTEST NAMES gtest)
find_library(GTEST_MAIN NAMES gtest_main)
find_path(GTEST_INCLUDE "gtest/gtest.h")
target_include_directories(tests PRIVATE ${GTEST_INCLUDE})

target_link_libraries(tests PRIVATE ${GTEST} ${GTEST_MAIN} -pthread)

install(TARGETS tests DESTINATION .)
