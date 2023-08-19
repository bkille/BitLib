# specify the C++ standard
cmake_minimum_required(VERSION 3.14)

# set the project name
project(Bit-Vector VERSION 0.1.1)

# set output directory of builds
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set CXX standard
# Things seem to be faster in cxx 20, and there is also std::shift_* 
# Should fall back on 17 if 20 is not supported
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to be used")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set up linters/checkers
#set(CMAKE_CXX_CPPCHECK cppcheck;--std=c++17;--file-filter=*bitlib*)
#set(CMAKE_CXX_CPPLINT cpplint;--linelength=100;--filter=-whitespace;)
#set(CMAKE_CXX_CLANG_TIDY 
  #clang-tidy;
  #-header-filter=include/;)


add_library(bitlib INTERFACE)
add_library(bitlib::bitlib ALIAS bitlib)
target_include_directories(bitlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# specify global compiler flags
include_directories("include/" "utils/" )

# Add fmt library (useful for printing words in binary and other debugging stuff)
#include(FetchContent)
#FetchContent_Declare(
    #fmt
    #GIT_REPOSITORY      https://github.com/fmtlib/fmt.git
    #GIT_TAG             e57ca2e3685b160617d3d95fcd9e789c4e06ca88 #v10.1.0
#)
#FetchContent_MakeAvailable(fmt)


option(BITLIB_HWY "Build with google highway SIMD extensions" OFF)
option(BITLIB_BENCHMARK "Build bitlib benchmarks" OFF)
option(BITLIB_EXAMPLE "Build bitlib examples" OFF)
option(BITLIB_TEST "Build bitlib tests" OFF)
option(BITLIB_COVERAGE "Compute test coverage" OFF)

if (BITLIB_HWY)
    add_definitions(-DBITLIB_HWY)
endif()

if(BITLIB_BENCHMARK)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    add_subdirectory(benchmark)
endif()
if(BITLIB_EXAMPLE)
    add_subdirectory(example)
endif()
if(BITLIB_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

