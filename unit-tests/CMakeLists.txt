include_directories (../src)

add_executable (unit_test ${SOURCES} ${UNIT_TEST_SOURCES} ${UNIT_TEST_HEADERS})
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
add_test(AllUnitTests unit_test)
