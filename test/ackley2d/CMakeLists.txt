set (simple_test_src ackley2d_main.cpp)

add_executable (bench_ackley2d ${simple_test_src})

target_link_libraries (bench_ackley2d revolution)
set_target_properties(bench_ackley2d PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMMON_OUTPUT_DIR})
