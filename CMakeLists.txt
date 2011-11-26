cmake_minimum_required (VERSION 2.6)

project (revolution)

set (CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Weffc++")

set(COMMON_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
add_subdirectory (src)
add_subdirectory (doxygen)


enable_testing()
add_subdirectory (test)
