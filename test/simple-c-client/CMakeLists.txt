set (simple_test_c_src main.c)

add_executable (main_c ${simple_test_c_src})
set_target_properties(main_c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMMON_OUTPUT_DIR})

target_link_libraries (main_c revolution)
