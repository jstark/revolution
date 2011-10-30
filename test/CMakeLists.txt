include_directories (../src)
add_subdirectory (simple-cpp-client)
add_subdirectory (simple-c-client)

add_test (simple-cpp-client ${CMAKE_CURRENT_BINARY_DIR}/simple-cpp-client/main)
set_tests_properties(simple-cpp-client PROPERTIES PASS_REGULAR_EXPRESSION "API: 0.0.0\nAPI: 0.0.0")
add_test (simple-c-client ${CMAKE_CURRENT_BINARY_DIR}/simple-c-client/main_c "API: 0.0.0\nAPI: 0.0.0")
