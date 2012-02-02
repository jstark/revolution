include_directories (../src)
add_subdirectory (simple-cpp-client)
add_subdirectory (simple-c-client)
add_subdirectory (himmelblau)
add_subdirectory (ackley2d)

add_test (simple-cpp-client ${COMMON_OUTPUT_DIR}/main)
add_test(himmelblau ${COMMON_OUTPUT_DIR}/bench_himmelblau)
add_test(ackley2d ${COMMON_OUTPUT_DIR}/bench_ackley2d)
