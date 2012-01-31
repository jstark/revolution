cmake_minimum_required (VERSION 2.6)

project (revolution)

set (CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

set (HEADERS 
	${CMAKE_SOURCE_DIR}/src/revolution.h
    ${CMAKE_SOURCE_DIR}/src/basic_es.h
	${CMAKE_SOURCE_DIR}/src/version.h
	${CMAKE_SOURCE_DIR}/src/objective_function.h
	${CMAKE_SOURCE_DIR}/src/atom.h
	${CMAKE_SOURCE_DIR}/src/population.h
	${CMAKE_SOURCE_DIR}/src/cma_es.h
	${CMAKE_SOURCE_DIR}/src/de.h
	${CMAKE_SOURCE_DIR}/src/array.h)

set (SOURCES 
	${CMAKE_SOURCE_DIR}/src/revolution.cpp
    ${CMAKE_SOURCE_DIR}/src/basic_es.cpp
	${CMAKE_SOURCE_DIR}/src/version.cpp
	${CMAKE_SOURCE_DIR}/src/objective_function.cpp
	${CMAKE_SOURCE_DIR}/src/atom.cpp
	${CMAKE_SOURCE_DIR}/src/cma_es.cpp
	${CMAKE_SOURCE_DIR}/src/de.cpp)
		
set (UNIT_TEST_SOURCES
	${CMAKE_SOURCE_DIR}/unit-tests/unit_test.cpp)
	
set (UNIT_TEST_HEADERS
	${CMAKE_SOURCE_DIR}/unit-tests/ut_version.h
	${CMAKE_SOURCE_DIR}/unit-tests/ut_basic_es.h
	${CMAKE_SOURCE_DIR}/unit-tests/ut_objective_function.h
	${CMAKE_SOURCE_DIR}/unit-tests/ut_atom.h
	${CMAKE_SOURCE_DIR}/unit-tests/ut_population.h)
	

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
