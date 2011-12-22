include_directories (../src)

set (SOURCES 
    ${CMAKE_SOURCE_DIR}/src/version.cpp
    ${CMAKE_SOURCE_DIR}/src/atom.cpp
    ${CMAKE_SOURCE_DIR}/src/revolution.cpp
    ${CMAKE_SOURCE_DIR}/src/objective_function.cpp
    ${CMAKE_SOURCE_DIR}/src/basic_es.cpp
)

add_executable (unit_test unit_test.cpp ${SOURCES})
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
#target_link_libraries(unit_test revolution)
add_test(AllUnitTests unit_test)
