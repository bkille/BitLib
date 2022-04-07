set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
set(DBENCHMARK_ENABLE_GTEST_TESTS OFF)
set(DEBUG_FLAGS -O0 -g -fno-omit-frame-pointer -pg)
cmake_minimum_required(VERSION 3.14)

# set the project name
project(Bit-Vector VERSION 0.1)

# set output directory of builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# specify the C++ standard
set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED true)

# specify global compiler flags
add_compile_options(-std=c++20 -march=native -pedantic -Wall -Wextra -Wfatal-errors)
include_directories("include/" "ext/simdpp")

# Add targets
file(GLOB TEST_SOURCES "src/unit-tests/*.cpp")
add_executable(tests ${TEST_SOURCES})
file(GLOB BENCH_SOURCES "src/benchmarks/*.cc")
add_executable(bench ${BENCH_SOURCES})
# Example targets
file(GLOB EXAMPLE1_SOURCES "example/print.cpp")
add_executable(example_print ${EXAMPLE1_SOURCES})

############
# Examples #
############
target_compile_options(example_print PUBLIC ${DEBUG_FLAGS})
install(TARGETS example_print DESTINATION .)

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

# specify test-specific libraries
include_directories(${googlebench_SOURCE_DIR}/benchmark/include src/utils)
target_link_libraries(bench PRIVATE benchmark::benchmark -pthread)

target_compile_options(bench PUBLIC -O3 -DNDEBUG -mtune=native)
install(TARGETS bench DESTINATION .)


###########
# Testing #
###########
# specify test-specific flags
target_compile_options(tests PUBLIC ${DEBUG_FLAGS})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

# specify test-specific libraries
include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest)
target_link_libraries(tests PRIVATE gtest gtest_main -pthread)

install(TARGETS tests DESTINATION .)

enable_testing()
add_test(NAME VectorTests COMMAND tests)
add_custom_target(check
    COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
    DEPENDS tests)


