set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(DEBUG_FLAGS -O0 -g -fno-omit-frame-pointer -pg -fprofile-arcs -ftest-coverage -fsanitize=address -Wextra-semi)
# specify the C++ standard
cmake_minimum_required(VERSION 3.14)

# set the project name
project(Bit-Vector VERSION 0.1.1)

# set output directory of builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set build type
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# set CXX standard
# Things seem to be faster in cxx 20, and there is also std::shift_* 
# Should fall back on 17 if 20 is not supported
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

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

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # We're in the root, define additional targets for developers.

    # specify global compiler flags
    add_compile_options(-march=native -pedantic -Wall -Wextra -Wfatal-errors)
    include_directories("include/" "src/utils/" "ext/simdpp")

    # Add fmt library (useful for printing words in binary and other debugging stuff)
    include(FetchContent)
    FetchContent_Declare(
        fmt
        URL https://github.com/fmtlib/fmt/archive/refs/tags/8.1.1.tar.gz
    )
    FetchContent_MakeAvailable(fmt)

    option(BITLIB_BENCHMARK "Build bitlib benchmarks" OFF)
    option(BITLIB_EXAMPLE "Build bitlib examples" OFF)
    option(BITLIB_TEST "Build bitlib tests" OFF)

    if(BITLIB_BENCHMARK)
        add_subdirectory(benchmark)
    endif()
    if(BITLIB_EXAMPLE)
        add_subdirectory(example)
    endif()
    if(BITLIB_TEST)
        add_subdirectory(test)
    endif()
endif()

