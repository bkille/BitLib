set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

find_package(benchmark REQUIRED)

# set output directory of builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set build type
set(CMAKE_BUILD_TYPE Release)

# Add targets
file(GLOB BENCH_SOURCES "src/*.cc")
add_executable(bitlib-bench ${BENCH_SOURCES})

add_subdirectory(ext/dynamic_bitset)

# specify benchmark-specific libraries
include_directories(
    ${googlebench_SOURCE_DIR}/benchmark/include 
    src/utils
    ext/BitArray 
    ext/itsy_bitsy/include)
target_link_libraries(bitlib-bench PRIVATE benchmark::benchmark -pthread ${CMAKE_CURRENT_LIST_DIR}/ext/BitArray/libbitarr.a sul::dynamic_bitset)

target_compile_options(bitlib-bench PUBLIC -O3 -DNDEBUG -march=native -Wpedantic)
install(TARGETS bitlib-bench DESTINATION .)
