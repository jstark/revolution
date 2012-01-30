include_directories (../src)

set (SOURCES 
    ${CMAKE_SOURCE_DIR}/src/version.cpp
    ${CMAKE_SOURCE_DIR}/src/atom.cpp
    ${CMAKE_SOURCE_DIR}/src/revolution.cpp
    ${CMAKE_SOURCE_DIR}/src/objective_function.cpp
    ${CMAKE_SOURCE_DIR}/src/basic_es.cpp
	${CMAKE_SOURCE_DIR}/src/cma_es.cpp
	${CMAKE_SOURCE_DIR}/src/de.cpp
)

add_executable (unit_test unit_test.cpp ${SOURCES})
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
add_test(AllUnitTests unit_test)
