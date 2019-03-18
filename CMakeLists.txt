cmake_minimum_required(VERSION 3.10)
project(chained_comparisons)

set(CMAKE_CXX_STANDARD 11)

add_library(chained_comparisons STATIC "${CMAKE_SOURCE_DIR}/include/chained_comparisons.h")
set_target_properties(chained_comparisons PROPERTIES LINKER_LANGUAGE CXX)

add_executable(chained_comparisons_test "${CMAKE_SOURCE_DIR}/test/test.cpp")
include_directories(chained_comparisons_test "${CMAKE_SOURCE_DIR}/include/")

enable_testing()
add_test(NAME test_chained_comparisons COMMAND chained_comparisons_test)
