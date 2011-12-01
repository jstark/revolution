include_directories (../src)
add_subdirectory (simple-cpp-client)
add_subdirectory (simple-c-client)
add_subdirectory (himmelblau)

add_test (simple-cpp-client ${COMMON_OUTPUT_DIR}/main)
#set_tests_properties(simple-cpp-client PROPERTIES PASS_REGULAR_EXPRESSION "API: 0.0.0\nAPI: 0.0.0")
add_test (simple-c-client ${COMMON_OUTPUT_DIR}/main_c "API: 0.0.0\nAPI: 0.0.0")

add_test(himmelblau ${COMMON_OUTPUT_DIR}/bench_himmelblau)
