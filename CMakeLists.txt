# cmake -S . -B build
# cmake --build build

#option(ENABLE_TIDY "Build with tidy checks" ON)
option(BUILD_CODEGRADE_TESTS "Build test suites into separate executables" OFF)

cmake_minimum_required(VERSION 3.10)
project(hw2 LANGUAGES C CXX)
find_package(GTest REQUIRED)

# if (ENABLE_TIDY)
#   set(CMAKE_CXX_CLANG_TIDY
#   clang-tidy;
#   -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*,bugprone-*,performance-*,readability-*,misc-*,-bugprone-easily-swappable-parameters,-bugprone-suspicious-include,-readability-magic-numbers,-readability-duplicate-include,
#   -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
#   )
# endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Build main executable
add_executable(hw2_main src/hw2_main.c)
target_compile_options(hw2_main PUBLIC -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(hw2_main PRIVATE m)
target_include_directories(hw2_main PUBLIC include)

# Build standalone test case suites for CodeGrade. These are separate executables so that CodeGrade can run them individually.
file(GLOB SOURCES tests/src/tests_*.cpp)
set(TEST_SUITES "combined_operations" "copy_paste" "load_save" "printing" "validate_args" "combined_operations_valgrind" "copy_paste_valgrind" "load_save_valgrind" "printing_valgrind")
if (BUILD_CODEGRADE_TESTS)
  foreach(TEST_SUITE IN LISTS TEST_SUITES)
    add_executable(tests_${TEST_SUITE} tests/src/tests_${TEST_SUITE}.cpp tests/src/tests_aux.cpp)
    target_compile_options(tests_${TEST_SUITE} PRIVATE -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
    target_include_directories(tests_${TEST_SUITE} PUBLIC include tests/include)
    target_link_libraries(tests_${TEST_SUITE} PRIVATE gtest gtest_main pthread m)
  endforeach()
else()
# Build a single executable with all the tests. Used during development only, not on CodeGrade.
  add_executable(run_all_tests ${SOURCES})
  target_compile_options(run_all_tests PRIVATE -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
  target_include_directories(run_all_tests PUBLIC include tests/include)
  target_link_libraries(run_all_tests PRIVATE gtest gtest_main pthread m)
endif()

