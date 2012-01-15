cmake_minimum_required (VERSION 2.6)

project (revolution)

set (CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

set(COMMON_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
add_subdirectory (src)
add_subdirectory (doxygen)

set (CMAKE_SKIP_BUILD_RPATH TRUE)
set (CMAKE_INSTALL_RPATH ".")
set (CMAME_BUILD_WITH_INSTALL_RPATH TRUE)

enable_testing()
find_package(GTest)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_subdirectory (unit-tests)
endif(GTEST_FOUND)
add_subdirectory (test)
