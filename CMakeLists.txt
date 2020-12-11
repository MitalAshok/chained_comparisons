cmake_minimum_required(VERSION 3.10)
project(chained_comparisons)

set(CMAKE_CXX_STANDARD 11)

add_library(chained_comparisons INTERFACE)
target_sources(chained_comparisons INTERFACE "${CMAKE_SOURCE_DIR}/include/chained_comparisons.h")
target_include_directories(chained_comparisons INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)


add_executable(chained_comparisons_test "${CMAKE_SOURCE_DIR}/test/test.cpp")
target_link_libraries(chained_comparisons_test PRIVATE chained_comparisons)

enable_testing()
add_test(NAME test_chained_comparisons COMMAND chained_comparisons_test)
