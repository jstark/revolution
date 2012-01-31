include_directories (../src)
add_executable (unit_exec ${SOURCES} ${UNIT_TEST_HEADERS} ${UNIT_TEST_SOURCES})
target_link_libraries(unit_exec ${GTEST_BOTH_LIBRARIES})
add_test(AllUnitTests unit_exec)
