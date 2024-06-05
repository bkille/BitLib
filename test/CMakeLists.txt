
###########
# Testing #
###########

find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB TEST_SOURCES "src/*.cpp")
add_executable(bitlib-tests ${TEST_SOURCES})

target_compile_options(bitlib-tests PUBLIC -O0 -g -fno-omit-frame-pointer -Wpedantic -fno-inline)

if (BITLIB_COVERAGE)
    target_compile_options(bitlib-tests PUBLIC --coverage -fprofile-arcs -ftest-coverage -pg)
endif()

# ASAN throws no matter what when using Google HWY, not sure why but need to fix
if (NOT BITLIB_HWY)
    target_compile_options(bitlib-tests PUBLIC -fsanitize=address)
    target_link_options(bitlib-tests PUBLIC -fsanitize=address)
endif()

# specify test-specific libraries
include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest src/utils)
target_link_libraries(bitlib-tests PUBLIC GTest::gtest GTest::gtest_main -pthread -lgcov --coverage)

if (NOT BITLIB_GTEST_REPEAT)
    set(BITLIB_GTEST_REPEAT 1)
endif()

enable_testing()
gtest_discover_tests(
    bitlib-tests
    EXTRA_ARGS --gtest_repeat=${BITLIB_GTEST_REPEAT})

#add_test(bitlib-check
    #COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bitlib-tests
    #DEPENDS bitlib-tests)
