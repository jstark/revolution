set (simple_test_src main.cpp)

add_executable (bench_himmelblau ${simple_test_src})

target_link_libraries (bench_himmelblau revolution)
set_target_properties(bench_himmelblau PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMMON_OUTPUT_DIR})
