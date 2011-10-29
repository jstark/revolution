cmake_minimum_required (VERSION 2.6)

project (revolution)

include_directories (include)

add_subdirectory (src)
add_subdirectory (doxygen)

enable_testing()
add_subdirectory (test)
