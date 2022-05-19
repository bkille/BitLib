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

# 
# specify global compiler flags
add_compile_options(-march=native -pedantic -Wall -Wextra -Wfatal-errors)
include_directories("include/" "ext/simdpp")

# Add targets
file(GLOB_RECURSE BITLIB_HEADERS "include/bitlib/" "*.hpp")
file(GLOB TEST_SOURCES "src/unit-tests/*.cpp")
add_executable(bitlib-tests ${TEST_SOURCES})
file(GLOB BENCH_SOURCES "src/benchmarks/*.cpp")
add_executable(bitlib-bench ${BENCH_SOURCES})
# Example targets
file(GLOB EXAMPLE1_SOURCES "example/print.cpp")
file(GLOB EXAMPLE2_SOURCES "example/timing.cpp")
add_executable(bitlib-example-print ${EXAMPLE1_SOURCES})
add_executable(bitlib-example-timing ${EXAMPLE2_SOURCES})
add_library(bitlib INTERFACE)
add_library(bitlib::bitlib ALIAS bitlib)

###########
# Library #
###########
target_include_directories(bitlib INTERFACE include/ ext/simdpp)
target_sources(bitlib INTERFACE ${BITLIB_HEADERS})

############
# Examples #
############
target_compile_options(bitlib-example-print PUBLIC)
install(TARGETS bitlib-example-print DESTINATION .)

################
# Benchmarking #
################
include(FetchContent)
FetchContent_Declare(
    googlebench
    URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googlebench)

add_library(bitarr STATIC "ext/BitArray/bit_array.c")
add_subdirectory("ext/sul_dynamic_bitset")
# specify benchmark-specific libraries
include_directories(${googlebench_SOURCE_DIR}/benchmark/include src/utils "ext/BitArray/")
target_link_libraries(bitlib-bench PRIVATE benchmark::benchmark_main benchmark::benchmark -pthread bitarr sul::dynamic_bitset)

target_compile_options(bitlib-bench PUBLIC -O3 -DNDEBUG -mtune=native)
install(TARGETS bitlib-bench DESTINATION .)


###########
# Testing #
###########
# specify test-specific flags
target_compile_options(bitlib-tests PUBLIC ${DEBUG_FLAGS})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

# specify test-specific libraries
include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest src/utils)
target_link_libraries(bitlib-tests PRIVATE gtest gtest_main -pthread -lgcov --coverage -fsanitize=address)

install(TARGETS bitlib-tests DESTINATION .)

enable_testing()
add_custom_target(bitlib-check
    COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bitlib-tests
    DEPENDS bitlib-tests)


