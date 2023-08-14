set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(DEBUG_FLAGS -O0 -g -fno-omit-frame-pointer -pg -fprofile-arcs -ftest-coverage -fsanitize=address -Wextra-semi)
# specify the C++ standard
cmake_minimum_required(VERSION 3.14)

find_package(benchmark REQUIRED)

# set output directory of builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set build type
set(CMAKE_BUILD_TYPE Release)

# Add targets
file(GLOB BENCH_SOURCES "src/*.cc")
add_executable(bitlib-bench ${BENCH_SOURCES})

# specify benchmark-specific libraries
include_directories(${googlebench_SOURCE_DIR}/benchmark/include src/utils)
target_link_libraries(bitlib-bench PRIVATE benchmark::benchmark -pthread)

target_compile_options(bitlib-bench PUBLIC -O3 -DNDEBUG -mtune=native)
install(TARGETS bitlib-bench DESTINATION .)
