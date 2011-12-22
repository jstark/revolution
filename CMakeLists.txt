cmake_minimum_required (VERSION 2.6)

project (revolution)

set (CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

set(COMMON_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
add_subdirectory (src)
add_subdirectory (doxygen)


enable_testing()
find_package(GTest)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_subdirectory (unit-tests)
endif(GTEST_FOUND)
add_subdirectory (test)
