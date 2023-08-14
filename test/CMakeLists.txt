
###########
# Testing #
###########

find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB TEST_SOURCES "src/*.cpp")
add_executable(bitlib-tests ${TEST_SOURCES})

set(DEBUG_FLAGS -O0 -g -fno-omit-frame-pointer -pg -fprofile-arcs -ftest-coverage -fsanitize=address -Wextra-semi --coverage -fno-inline -fno-inline-small-functions -fno-default-inline)

# specify test-specific flags
target_compile_options(bitlib-tests PUBLIC ${DEBUG_FLAGS})

# specify test-specific libraries
include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest src/utils)
target_link_libraries(bitlib-tests PRIVATE gtest gtest_main -pthread -lgcov --coverage -fsanitize=address)

set(BITLIB_GTEST_REPEAT 1)

enable_testing()
gtest_discover_tests(
    bitlib-tests
    EXTRA_ARGS --gtest_repeat=${BITLIB_GTEST_REPEAT})

#add_test(bitlib-check
    #COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bitlib-tests
    #DEPENDS bitlib-tests)
